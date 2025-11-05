#include "SpriteSystem.h"

#include "TransformComponent2D.h"
#include "TransformSystem2D.h"
#include "SpriteComponent.h"
#include "SpriteVertex.h"
#include "CameraComponent2D.h"
#include "CameraSystem2D.h"
#include "RenderObject.h"
#include "Utils/FileUtils.h"
#include "Camera2D.h"
#include "Renderer2DSystem.h"
#include "PekanLogger.h"

using namespace Pekan::Graphics;

#define VERTEX_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shaders/2D_Sprite_VertexShader.glsl"
#define FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shaders/2D_Sprite_FragmentShader.glsl"

namespace Pekan
{
namespace Renderer2D
{

	// Computes local vertex positions for a given sprite
    static void getVerticesLocal(const SpriteComponent& sprite, glm::vec2* verticesLocal /* output array of 4 vec2's */)
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
		SpriteVertex* verticesWorld    // output array of 4 SpriteVertex's
    )
    {
        glm::vec2 verticesLocal[4];
        getVerticesLocal(sprite, verticesLocal);

        const glm::mat3& worldMatrix = TransformSystem2D::getWorldMatrix(registry, transform);
        // Calculate world vertex positions by applying the world matrix to the local vertex positions
        verticesWorld[0].position = glm::vec2(worldMatrix * glm::vec3(verticesLocal[0], 1.0f));
        verticesWorld[1].position = glm::vec2(worldMatrix * glm::vec3(verticesLocal[1], 1.0f));
        verticesWorld[2].position = glm::vec2(worldMatrix * glm::vec3(verticesLocal[2], 1.0f));
        verticesWorld[3].position = glm::vec2(worldMatrix * glm::vec3(verticesLocal[3], 1.0f));

        // Set "textureCoordinates" attribute of each vertex
        PK_ASSERT
        (
            sprite.textureCoordinatesMin.x < sprite.textureCoordinatesMax.x
            && sprite.textureCoordinatesMin.y < sprite.textureCoordinatesMax.y,
            "Trying to render an entity with a SpriteComponent that has invalid texture coordinates.", "Pekan"
        );
        verticesWorld[0].textureCoordinates = { sprite.textureCoordinatesMin.x, sprite.textureCoordinatesMin.y };
        verticesWorld[1].textureCoordinates = { sprite.textureCoordinatesMax.x, sprite.textureCoordinatesMin.y };
        verticesWorld[2].textureCoordinates = { sprite.textureCoordinatesMax.x, sprite.textureCoordinatesMax.y };
        verticesWorld[3].textureCoordinates = { sprite.textureCoordinatesMin.x, sprite.textureCoordinatesMax.y };
    }

    // Sets "uViewProjectionMatrix" uniform inside a given shader using a given camera component
    static void setViewProjectionMatrixUniform(Shader& shader, const CameraComponent2D& camera)
    {
        const glm::mat4& viewProjectionMatrix = camera.getViewProjectionMatrix();
        shader.setUniformMatrix4fv("uViewProjectionMatrix", viewProjectionMatrix);
    }

	// Creates a render object for a given sprite
    static void createRenderObjectForSprite
    (
        const entt::registry& registry,
        const SpriteComponent& sprite,
        const TransformComponent2D& transform,
        int textureSlot,              // texture slot to set in the shader uniform
        RenderObject& renderObject    // render object to create
    )
    {
		// Get sprite's world vertices
        SpriteVertex verticesWorld[4];
        getVerticesWorld(registry, sprite, transform, verticesWorld);
		// Create render object with sprite's world vertices
        renderObject.create
        (
            verticesWorld,
            sizeof(SpriteVertex) * 4,
            {
                { ShaderDataType::Float2, "position" },
                { ShaderDataType::Float2, "textureCoordinates" }
            },
            BufferDataUsage::DynamicDraw,
            FileUtils::readTextFileToString(VERTEX_SHADER_FILEPATH).c_str(),
            FileUtils::readTextFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
        );
        // Set render object's index data for a sprite formed by two triangles
        static constexpr unsigned indices[6] = { 0, 1, 2, 0, 2, 3 };
        renderObject.setIndexData(indices, sizeof(unsigned) * 6);

		// Set render object's shader uniforms
        {
            Shader& shader = renderObject.getShader();
            // Set view projection matrix uniform using the primary camera
            const CameraComponent2D& camera = CameraSystem2D::getPrimaryCamera(registry);
            setViewProjectionMatrixUniform(shader, camera);
			// Set texture slot uniform
            shader.setUniform1i("uTexture", textureSlot);
        }
    }

    void SpriteSystem::render(const entt::registry& registry)
    {
        // Get a view of all entities with sprite and 2D transform components
        const auto view = registry.view<SpriteComponent, TransformComponent2D>();
        // Render each such entity
        for (entt::entity entity : view)
        {
            render(registry, entity);
        }
    }

	void SpriteSystem::render(const entt::registry& registry, entt::entity entity)
	{
        PK_ASSERT(registry.valid(entity), "Trying to render an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<SpriteComponent>(entity), "Trying to render an entity that doesn't have a SpriteComponent component.", "Pekan");
        PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to render an entity that doesn't have a TransformComponent2D component.", "Pekan");

		// Get entity's sprite and transform components
		const SpriteComponent& sprite = registry.get<SpriteComponent>(entity);
		const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);
		// Create render object for the sprite
        RenderObject renderObject;
		createRenderObjectForSprite(registry, sprite, transform, 0, renderObject);
		// Bind sprite's texture
        sprite.texture->bind(0);
		// Render sprite's render object
        renderObject.render();
	}

    float SpriteSystem::getWidth(const entt::registry& registry, entt::entity entity)
    {
        PK_ASSERT(registry.valid(entity), "Trying to get width of an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<SpriteComponent>(entity), "Trying to get width of an entity that doesn't have a SpriteComponent component.", "Pekan");

        const SpriteComponent& sprite = registry.get<SpriteComponent>(entity);
        return sprite.width;
    }

    float SpriteSystem::getHeight(const entt::registry& registry, entt::entity entity)
    {
        PK_ASSERT(registry.valid(entity), "Trying to get height of an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<SpriteComponent>(entity), "Trying to get height of an entity that doesn't have a SpriteComponent component.", "Pekan");

        const SpriteComponent& sprite = registry.get<SpriteComponent>(entity);
        return sprite.height;
    }

    glm::vec2 SpriteSystem::getSize(const entt::registry& registry, entt::entity entity)
    {
        PK_ASSERT(registry.valid(entity), "Trying to get size of an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<SpriteComponent>(entity), "Trying to get size of an entity that doesn't have a SpriteComponent component.", "Pekan");

        const SpriteComponent& sprite = registry.get<SpriteComponent>(entity);
        return { sprite.width, sprite.height };
    }

    Graphics::Texture2D_ConstPtr SpriteSystem::getTexture(const entt::registry& registry, entt::entity entity)
    {
        PK_ASSERT(registry.valid(entity), "Trying to get texture of an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<SpriteComponent>(entity), "Trying to get texture of an entity that doesn't have a SpriteComponent component.", "Pekan");

        const SpriteComponent& sprite = registry.get<SpriteComponent>(entity);
        return sprite.texture;
    }

    glm::vec2 SpriteSystem::getTextureCoordinatesMin(const entt::registry& registry, entt::entity entity)
    {
        PK_ASSERT(registry.valid(entity), "Trying to get texture coordinates min of an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<SpriteComponent>(entity), "Trying to get texture coordinates min of an entity that doesn't have a SpriteComponent component.", "Pekan");

        const SpriteComponent& sprite = registry.get<SpriteComponent>(entity);
        return sprite.textureCoordinatesMin;
    }

    glm::vec2 SpriteSystem::getTextureCoordinatesMax(const entt::registry& registry, entt::entity entity)
    {
        PK_ASSERT(registry.valid(entity), "Trying to get texture coordinates max of an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<SpriteComponent>(entity), "Trying to get texture coordinates max of an entity that doesn't have a SpriteComponent component.", "Pekan");

        const SpriteComponent& sprite = registry.get<SpriteComponent>(entity);
        return sprite.textureCoordinatesMax;
    }

} // namespace Renderer2D
} // namespace Pekan
