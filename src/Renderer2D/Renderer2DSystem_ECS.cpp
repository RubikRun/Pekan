#include "Renderer2DSystem_ECS.h"

#include "TransformComponent2D.h"
#include "RectangleGeometryComponent.h"
#include "RectangleGeometrySystem.h"
#include "LineGeometryComponent.h"
#include "LineGeometrySystem.h"
#include "CircleGeometryComponent.h"
#include "CircleGeometrySystem.h"
#include "TriangleGeometryComponent.h"
#include "TriangleGeometrySystem.h"
#include "SolidColorMaterialComponent.h"
#include "SolidColorMaterialSystem.h"
#include "SpriteSystem.h"
#include "RenderObject.h"
#include "Renderer2DSystem.h"
#include "Camera2D.h"
#include "Utils/FileUtils.h"
#include "Utils/MathUtils.h"
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

    // Type alias for a vertex positions getter function
    using VertexPositionsGetter = void(*)
    (
        const entt::registry& registry, entt::entity entity,
        void* vertices, int verticesCount, int vertexSize, int positionAttributeOffset
    );

    // Renders an entity with a shape geometry and a solid color material
    static void renderShapeWithSolidColorMaterial
    (
        const entt::registry& registry,
        entt::entity entity,
        VertexPositionsGetter vertexPositionsGetter,    // a function for getting shape's vertex positions
        int verticesCount,                              // number of shape's vertices
        const unsigned* indices,                        // indices array
        int indicesCount                                // number of indices
    )
    {
        // Create vertices array with given number of vertices
        std::vector<VertexOfShapeWithSolidColorMaterial> vertices(verticesCount);

        // Get vertex positions into the position attribute of vertices array
        vertexPositionsGetter
        (
            registry, entity,
            vertices.data(), verticesCount,
            sizeof(VertexOfShapeWithSolidColorMaterial),
            offsetof(VertexOfShapeWithSolidColorMaterial, position)
        );
        // Get vertex colors into the color attribute of vertices array
        SolidColorMaterialSystem::getVertexColors
        (
            registry, entity,
            vertices.data(), verticesCount,
            sizeof(VertexOfShapeWithSolidColorMaterial),
            offsetof(VertexOfShapeWithSolidColorMaterial, color)
        );

        // Create render object from shape's vertices and indices
        RenderObject renderObject;
        createRenderObjectForShapeWithSolidColorMaterial
        (
            vertices.data(), verticesCount,
            indices, indicesCount,
            renderObject
        );

        // Render shape's render object
        renderObject.render();
    }

    // Renders an entity with rectangle geometry and a solid color material
    static void renderRectangleWithSolidColorMaterial(const entt::registry& registry, entt::entity entity)
    {
        // Define indices for two triangles making up a rectangle
        static constexpr unsigned indices[6] = { 0, 1, 2, 0, 2, 3 };

        // Render rectangle as a general shape with solid color material
        renderShapeWithSolidColorMaterial
        (
            registry, entity,
            [](const entt::registry& registry, entt::entity entity, void* vertices, int verticesCount, int vertexSize, int positionAttributeOffset)
            {
                RectangleGeometrySystem::getVertexPositions(registry, entity, vertices, vertexSize, positionAttributeOffset);
            },
            4,
            indices, 6
        );
    }

	// Renders all entities with rectangle geometry and a solid color material in a given registry
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

    // Renders an entity with line geometry and a solid color material
    static void renderLineWithSolidColorMaterial(const entt::registry& registry, entt::entity entity)
    {
        // Define indices for two triangles making up a rectangle
		// (since lines are rendered as thin rectangles)
        static constexpr unsigned indices[6] = { 0, 1, 2, 0, 2, 3 };

        // Render line as a general shape with solid color material
        renderShapeWithSolidColorMaterial
        (
            registry, entity,
            [](const entt::registry& registry, entt::entity entity, void* vertices, int verticesCount, int vertexSize, int positionAttributeOffset)
            {
                LineGeometrySystem::getVertexPositions(registry, entity, vertices, vertexSize, positionAttributeOffset);
            },
            4,
            indices, 6
        );
    }

    // Renders all entities with line geometry and a solid color material in a given registry
    static void renderLinesWithSolidColorMaterial(const entt::registry& registry)
    {
        // Get a view of all entities that have line geometry, 2D transform and solid color material components
        const auto view = registry.view<LineGeometryComponent, TransformComponent2D, SolidColorMaterialComponent>();
        // Render each such entity
        for (entt::entity entity : view)
        {
            renderLineWithSolidColorMaterial(registry, entity);
        }
    }

    // Renders an entity with circle geometry and a solid color material
    static void renderCircleWithSolidColorMaterial(const entt::registry& registry, entt::entity entity)
    {
        // Get number of vertices needed for entity's circle geometry
        const int verticesCount = CircleGeometrySystem::getNumberOfVertices(registry, entity);

        // Create indices array for triangle fan
        std::vector<unsigned> indices((verticesCount - 1) * 3);
        MathUtils::generateTriangleFanIndices(indices.data(), verticesCount);

        // Render circle as a general shape with solid color material
        renderShapeWithSolidColorMaterial
        (
            registry, entity,
            CircleGeometrySystem::getVertexPositions,
            verticesCount,
            indices.data(), int(indices.size())
        );
    }

    // Renders all entities with circle geometry and a solid color material in a given registry
    static void renderCirclesWithSolidColorMaterial(const entt::registry& registry)
    {
        // Get a view of all entities that have circle geometry, 2D transform and solid color material components
        const auto view = registry.view<CircleGeometryComponent, TransformComponent2D, SolidColorMaterialComponent>();
        // Render each such entity
        for (entt::entity entity : view)
        {
            renderCircleWithSolidColorMaterial(registry, entity);
        }
    }

    // Renders an entity with triangle geometry and a solid color material
    static void renderTriangleWithSolidColorMaterial(const entt::registry& registry, entt::entity entity)
    {
        // Define indices for a single triangle
        static constexpr unsigned indices[3] = { 0, 1, 2 };

        // Render triangle as a general shape with solid color material
        renderShapeWithSolidColorMaterial
        (
            registry, entity,
            [](const entt::registry& registry, entt::entity entity, void* vertices, int verticesCount, int vertexSize, int positionAttributeOffset)
            {
                TriangleGeometrySystem::getVertexPositions(registry, entity, vertices, vertexSize, positionAttributeOffset);
            },
            3,
            indices, 3
        );
    }

    // Renders all entities with triangle geometry and a solid color material in a given registry
    static void renderTrianglesWithSolidColorMaterial(const entt::registry& registry)
    {
        // Get a view of all entities that have triangle geometry, 2D transform and solid color material components
        const auto view = registry.view<TriangleGeometryComponent, TransformComponent2D, SolidColorMaterialComponent>();
        // Render each such entity
        for (entt::entity entity : view)
        {
            renderTriangleWithSolidColorMaterial(registry, entity);
        }
    }

    void Renderer2DSystem_ECS::render(const entt::registry& registry)
    {
        renderRectanglesWithSolidColorMaterial(registry);
        renderTrianglesWithSolidColorMaterial(registry);
        renderCirclesWithSolidColorMaterial(registry);
        renderLinesWithSolidColorMaterial(registry);
        SpriteSystem::render(registry);
    }

} // namespace Renderer2D
} // namespace Pekan
