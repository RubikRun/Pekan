#include "SpriteSystem.h"

#include "TransformComponent2D.h"
#include "TransformSystem2D.h"
#include "SpriteComponent.h"
#include "SpriteVertex.h"
#include "CameraComponent2D.h"
#include "CameraSystem2D.h"
#include "DrawObject.h"
#include "Utils/FileUtils.h"
#include "PekanLogger.h"
#include "Entity/DisabledComponent.h"

using namespace Pekan::Graphics;

#define VERTEX_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shaders/2D_Sprite_VertexShader.glsl"
#define FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shaders/2D_Sprite_FragmentShader.glsl"

namespace Pekan
{
namespace Renderer2D
{

    // Current primary camera cached here for easy access
    static const CameraComponent2D* g_camera = nullptr;

	// Computes local vertex positions for a given sprite
    static void getLocalVertexPositions(const SpriteComponent& sprite, glm::vec2* verticesLocal /* output array of 4 vec2's */)
    {
        verticesLocal[0] = glm::vec2(-sprite.width / 2.0f, -sprite.height / 2.0f);
        verticesLocal[1] = glm::vec2(sprite.width / 2.0f, -sprite.height / 2.0f);
        verticesLocal[2] = glm::vec2(sprite.width / 2.0f, sprite.height / 2.0f);
        verticesLocal[3] = glm::vec2(-sprite.width / 2.0f, sprite.height / 2.0f);
	}

    // Computes world vertices for a given sprite
    static void getVerticesWorld
    (
        const entt::registry& registry,
        const SpriteComponent& sprite,
        const TransformComponent2D& transform,
		SpriteVertex* vertices    // output array of 4 SpriteVertex's
    )
    {
        glm::vec2 localVertexPositions[4];
        getLocalVertexPositions(sprite, localVertexPositions);

        const glm::mat3& worldMatrix = TransformSystem2D::getWorldMatrix(registry, transform);
        // Calculate world vertex positions by applying the world matrix to the local vertex positions
        vertices[0].position = glm::vec2(worldMatrix * glm::vec3(localVertexPositions[0], 1.0f));
        vertices[1].position = glm::vec2(worldMatrix * glm::vec3(localVertexPositions[1], 1.0f));
        vertices[2].position = glm::vec2(worldMatrix * glm::vec3(localVertexPositions[2], 1.0f));
        vertices[3].position = glm::vec2(worldMatrix * glm::vec3(localVertexPositions[3], 1.0f));

        // Set "textureCoordinates" attribute of each vertex
        PK_ASSERT
        (
            sprite.textureCoordinatesMin.x < sprite.textureCoordinatesMax.x
            && sprite.textureCoordinatesMin.y < sprite.textureCoordinatesMax.y,
            "Cannot render an entity with a SpriteComponent because it has invalid texture coordinates.", "Pekan"
        );
        vertices[0].textureCoordinates = { sprite.textureCoordinatesMin.x, sprite.textureCoordinatesMin.y };
        vertices[1].textureCoordinates = { sprite.textureCoordinatesMax.x, sprite.textureCoordinatesMin.y };
        vertices[2].textureCoordinates = { sprite.textureCoordinatesMax.x, sprite.textureCoordinatesMax.y };
        vertices[3].textureCoordinates = { sprite.textureCoordinatesMin.x, sprite.textureCoordinatesMax.y };
    }

    // Computes local vertices for a given sprite
    static void getVerticesLocal
    (
        const entt::registry& registry,
        const SpriteComponent& sprite,
        SpriteVertex* vertices    // output array of 4 SpriteVertex's
    )
    {
        // Get local vertex positions from sprite
        glm::vec2 localVertexPositions[4];
        getLocalVertexPositions(sprite, localVertexPositions);

        // Set "position" attribute of each vertex to local vertex positions
        vertices[0].position = localVertexPositions[0];
        vertices[1].position = localVertexPositions[1];
        vertices[2].position = localVertexPositions[2];
        vertices[3].position = localVertexPositions[3];

        // Set "textureCoordinates" attribute of each vertex
        PK_ASSERT
        (
            sprite.textureCoordinatesMin.x < sprite.textureCoordinatesMax.x
            && sprite.textureCoordinatesMin.y < sprite.textureCoordinatesMax.y,
            "Cannot render an entity with a SpriteComponent because it has invalid texture coordinates.", "Pekan"
        );
        vertices[0].textureCoordinates = { sprite.textureCoordinatesMin.x, sprite.textureCoordinatesMin.y };
        vertices[1].textureCoordinates = { sprite.textureCoordinatesMax.x, sprite.textureCoordinatesMin.y };
        vertices[2].textureCoordinates = { sprite.textureCoordinatesMax.x, sprite.textureCoordinatesMax.y };
        vertices[3].textureCoordinates = { sprite.textureCoordinatesMin.x, sprite.textureCoordinatesMax.y };
    }

    // Sets "uViewProjectionMatrix" uniform inside a given shader using a given camera component
    static void setViewProjectionMatrixUniform(Shader& shader, const CameraComponent2D* camera)
    {
        PK_ASSERT(camera != nullptr, "Cannot set view projection matrix uniform without a camera.", "Pekan");
        const glm::mat4& viewProjectionMatrix = camera->getViewProjectionMatrix();
        shader.setUniformMatrix4fv("uViewProjectionMatrix", viewProjectionMatrix);
    }

	// Creates a draw object for a given sprite
    static void createDrawObjectForSprite
    (
        const entt::registry& registry,
        const SpriteComponent& sprite,
        const TransformComponent2D& transform,
        int textureSlot,              // texture slot to set in the shader uniform
        DrawObject& drawObject    // draw object to create
    )
    {
		// Get sprite's vertices
        SpriteVertex vertices[4];
        getVerticesWorld(registry, sprite, transform, vertices);
		// Create draw object with sprite's vertices
        drawObject.create
        (
            vertices,
            sizeof(SpriteVertex) * 4,
            {
                { ShaderDataType::Float2, "position" },
                { ShaderDataType::Float2, "textureCoordinates" }
            },
            BufferDataUsage::DynamicDraw,
            FileUtils::readTextFileToString(VERTEX_SHADER_FILEPATH).c_str(),
            FileUtils::readTextFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
        );
        // Set draw object's index data for a sprite formed by two triangles
        static constexpr unsigned indices[6] = { 0, 1, 2, 0, 2, 3 };
        drawObject.setIndexData(indices, sizeof(unsigned) * 6);

		// Set draw object's shader uniforms
        {
            Shader& shader = drawObject.getShader();
            // Set view projection matrix uniform using the primary camera
            setViewProjectionMatrixUniform(shader, g_camera);
			// Set texture slot uniform
            shader.setUniform1i("uTexture", textureSlot);
        }
    }

    // Creates a draw object for a given sprite
    static void createDrawObjectForSprite
    (
        const entt::registry& registry,
        const SpriteComponent& sprite,
        int textureSlot,              // texture slot to set in the shader uniform
        DrawObject& drawObject    // draw object to create
    )
    {
        // Get sprite's world vertices
        SpriteVertex vertices[4];
        getVerticesLocal(registry, sprite, vertices);
        // Create draw object with sprite's vertices
        drawObject.create
        (
            vertices,
            sizeof(SpriteVertex) * 4,
            {
                { ShaderDataType::Float2, "position" },
                { ShaderDataType::Float2, "textureCoordinates" }
            },
            BufferDataUsage::DynamicDraw,
            FileUtils::readTextFileToString(VERTEX_SHADER_FILEPATH).c_str(),
            FileUtils::readTextFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
        );
        // Set draw object's index data for a sprite formed by two triangles
        static constexpr unsigned indices[6] = { 0, 1, 2, 0, 2, 3 };
        drawObject.setIndexData(indices, sizeof(unsigned) * 6);

        // Set draw object's shader uniforms
        {
            Shader& shader = drawObject.getShader();
            // Set view projection matrix uniform using the primary camera
            setViewProjectionMatrixUniform(shader, g_camera);
            // Set texture slot uniform
            shader.setUniform1i("uTexture", textureSlot);
        }
    }

    // Renders an entity with a sprite component
    // @tparam HasTransform - A boolean parameter indicating if the entity has a transform component
    template<bool HasTransform>
    static void renderSprite(const entt::registry& registry, entt::entity entity);

    template<>
    static void renderSprite<true>(const entt::registry& registry, entt::entity entity)
    {
        PK_ASSERT(registry.valid(entity), "Cannot render an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<SpriteComponent>(entity), "Cannot render an entity that doesn't have a SpriteComponent.", "Pekan");
        PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Cannot render an entity that doesn't have a TransformComponent2D.", "Pekan");

        // Get entity's sprite and transform components
        const SpriteComponent& sprite = registry.get<SpriteComponent>(entity);
        const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);
        // Create draw object for the sprite
        DrawObject drawObject;
        createDrawObjectForSprite(registry, sprite, transform, 0, drawObject);
        // Bind sprite's texture
        sprite.texture->bind(0);
        // Render sprite's draw object
        drawObject.render();
    }

    template<>
    static void renderSprite<false>(const entt::registry& registry, entt::entity entity)
    {
        PK_ASSERT(registry.valid(entity), "Cannot render an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<SpriteComponent>(entity), "Cannot render an entity that doesn't have a SpriteComponent.", "Pekan");

        // Get entity's sprite component
        const SpriteComponent& sprite = registry.get<SpriteComponent>(entity);
        // Create draw object for the sprite
        DrawObject drawObject;
        createDrawObjectForSprite(registry, sprite, 0, drawObject);
        // Bind sprite's texture
        sprite.texture->bind(0);
        // Render sprite's draw object
        drawObject.render();
    }

    // Renders all sprites that have (or all sprites that don't have) a transform component
    // @tparam HasTransform - A boolean parameter indicating if the entities have a transform component
    template<bool HasTransform>
    static void renderAllSprites(const entt::registry& registry);

    template<>
    static void renderAllSprites<true>(const entt::registry& registry)
    {
        // Get a view of all entities with a sprite component and a transform component
        const auto view = registry.view<SpriteComponent, TransformComponent2D>(entt::exclude<DisabledComponent>);
        // Render each such entity
        for (entt::entity entity : view)
        {
            renderSprite<true>(registry, entity);
        }
    }

    template<>
    static void renderAllSprites<false>(const entt::registry& registry)
    {
        // Get a view of all entities with a sprite component but without a transform component
        const auto view = registry.view<SpriteComponent>(entt::exclude<DisabledComponent, TransformComponent2D>);
        // Render each such entity
        for (entt::entity entity : view)
        {
            renderSprite<false>(registry, entity);
        }
    }

    void SpriteSystem::render(const entt::registry& registry, const CameraComponent2D* camera)
    {
        PK_ASSERT_QUICK(camera != nullptr);
        g_camera = camera;

        renderAllSprites<true>(registry);
        renderAllSprites<false>(registry);
    }

} // namespace Renderer2D
} // namespace Pekan
