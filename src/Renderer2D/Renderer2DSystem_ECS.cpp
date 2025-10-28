#include "Renderer2DSystem_ECS.h"

#include "TransformComponent2D.h"
#include "RectangleGeometryComponent.h"
#include "RectangleGeometrySystem.h"
#include "SolidColorMaterialComponent.h"
#include "SolidColorMaterialSystem.h"
#include "SpriteSystem.h"
#include "RenderObject.h"
#include "Renderer2DSystem.h"
#include "Camera2D.h"
#include "Utils/FileUtils.h"
#include "PekanLogger.h"

using namespace Pekan::Graphics;

#define VERTEX_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shaders/2D_Shape_SolidColorMaterial_VertexShader.glsl"
#define FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shaders/2D_Shape_SolidColorMaterial_FragmentShader.glsl"

namespace Pekan
{
namespace Renderer2D
{

	// Structure defining the layout of a vertex of a shape with solid color material
    struct VertexOfShapeWithSolidColorMaterial
    {
        glm::vec2 position = { 0.0f, 0.0f };
        glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    };

    // Sets "uViewProjectionMatrix" uniform inside a given shader using a given camera
    static void setViewProjectionMatrixUniform(Shader& shader, const Camera2D_ConstPtr& camera)
    {
        if (camera != nullptr)
        {
            // Set shader's view projection matrix uniform to camera's view projection matrix
            const glm::mat4& viewProjectionMatrix = camera->getViewProjectionMatrix();
            shader.setUniformMatrix4fv("uViewProjectionMatrix", viewProjectionMatrix);
        }
        else
        {
            // Set shader's view projection matrix uniform to a default view projection matrix
            static constexpr glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
            shader.setUniformMatrix4fv("uViewProjectionMatrix", defaultViewProjectionMatrix);
        }
    }

    // Creates a render object from given vertices and indices of a shape with solid color material
	static void createRenderObjectForShapeWithSolidColorMaterial
    (
        const VertexOfShapeWithSolidColorMaterial* vertices, int verticesCount,
        const unsigned *indices, int indicesCount,
        RenderObject& renderObject    // render object to create
    )
    {
        // Create render object with given vertices
        renderObject.create
        (
            vertices,
            sizeof(VertexOfShapeWithSolidColorMaterial) * verticesCount,
            {
                { ShaderDataType::Float2, "position" },
                { ShaderDataType::Float4, "color" }
            },
            BufferDataUsage::DynamicDraw,
            FileUtils::readTextFileToString(VERTEX_SHADER_FILEPATH).c_str(),
            FileUtils::readTextFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
        );
        // Set given indices to the render object
        renderObject.setIndexData(indices, sizeof(unsigned) * indicesCount);

        // Set render object's shader uniforms
        {
            Shader& shader = renderObject.getShader();
            // Set view projection matrix uniform using active camera
            Camera2D_ConstPtr camera = Renderer2DSystem::getCamera();
            setViewProjectionMatrixUniform(shader, camera);
        }
    }

    static void renderRectangleWithSolidColorMaterial(const entt::registry& registry, entt::entity entity)
    {
        VertexOfShapeWithSolidColorMaterial vertices[4];
		// Fill vertices array with rectangle's vertex positions
		RectangleGeometrySystem::fillVertexPositions
        (
            registry, entity,
            vertices,
            sizeof(VertexOfShapeWithSolidColorMaterial),
            offsetof(VertexOfShapeWithSolidColorMaterial, position)
        );
		// Fill vertices array with rectangle's vertex colors
        SolidColorMaterialSystem::fillVertexColors
        (
            registry, entity,
            vertices, 4,
            sizeof(VertexOfShapeWithSolidColorMaterial),
            offsetof(VertexOfShapeWithSolidColorMaterial, color)
		);

        // Create render object from rectangle's vertices
        // and indices forming a rectangle from two triangles.
		static constexpr unsigned indices[6] = { 0, 1, 2, 0, 2, 3 };
        RenderObject renderObject;
        createRenderObjectForShapeWithSolidColorMaterial
        (
            vertices, 4,
            indices, 6,
            renderObject
		);

        // Render rectangle's render object
        renderObject.render();
	}

    static void renderRectanglesWithSolidColorMaterial(const entt::registry& registry)
    {
		// Get a view of all entities that have rectangle geometry, 2D transform and solid color material components
        const auto view = registry.view<RectangleGeometryComponent, TransformComponent2D, SolidColorMaterialComponent>();
        // Render each such entity
        for (entt::entity entity : view)
        {
			renderRectangleWithSolidColorMaterial(registry, entity);
        }
	}

	void Renderer2DSystem_ECS::render(const entt::registry& registry)
	{
        renderRectanglesWithSolidColorMaterial(registry);
        SpriteSystem::render(registry);
		// TODO: Add support for other geometry types with solid color material, and other material types
	}

} // namespace Renderer2D
} // namespace Pekan
