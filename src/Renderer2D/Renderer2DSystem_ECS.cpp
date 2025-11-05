#include "Renderer2DSystem_ECS.h"

#include "TransformComponent2D.h"
#include "SpriteSystem.h"

#include "RenderObject.h"
#include "CameraComponent2D.h"
#include "CameraSystem2D.h"

////////// Geometry components and systems //////////
#include "RectangleGeometryComponent.h"
#include "RectangleGeometrySystem.h"
#include "LineGeometryComponent.h"
#include "LineGeometrySystem.h"
#include "CircleGeometryComponent.h"
#include "CircleGeometrySystem.h"
#include "TriangleGeometryComponent.h"
#include "TriangleGeometrySystem.h"
#include "PolygonGeometryComponent.h"
#include "PolygonGeometrySystem.h"
//////////////////////////////////////////////////////

////////// Material components and systems //////////
#include "SolidColorMaterialComponent.h"
#include "SolidColorMaterialSystem.h"
/////////////////////////////////////////////////////

////////// Pekan Core includes //////////
#include "Utils/FileUtils.h"
#include "PekanLogger.h"
/////////////////////////////////////////

using namespace Pekan::Graphics;

#define VERTEX_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shaders/2D_Shape_SolidColorMaterial_VertexShader.glsl"
#define FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shaders/2D_Shape_SolidColorMaterial_FragmentShader.glsl"

namespace Pekan
{
namespace Renderer2D
{

    // Type alias for a vertex positions getter function
    using VertexPositionsGetter = void(*)
    (
        const entt::registry& registry, entt::entity entity,
        void* vertices, int vertexSize, int positionAttributeOffset
    );
    // Type alias for a vertex positions and indices getter function
    using VertexPositionsAndIndicesGetter = void(*)
    (
        const entt::registry& registry, entt::entity entity,
        void* vertices, int verticesCount, int vertexSize, int positionAttributeOffset,
        std::vector<unsigned>& indices
    );
    // Type alias for a function that renders an entity
    using RenderFunction = void(*)(const entt::registry&, entt::entity);

    // Structure defining the layout of a vertex of a shape with solid color material
    struct VertexOfShapeWithSolidColorMaterial
    {
        glm::vec2 position = { 0.0f, 0.0f };
        glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    };

    // Renders all entities with given component types
    // by calling a given render function for each entity
    //
    // @tparam ComponentTypes...  Component types that an entity must have to be rendered
    // @param[in] registry        Registry containing entities to render
    // @param[in] renderFunction  Function that renders a single entity
    template<typename... ComponentTypes>
    void renderAllEntitiesWith(const entt::registry& registry, RenderFunction renderFunction)
    {
        // Create a view over all entities that have the given components
        auto view = registry.view<ComponentTypes...>();
        // Iterate over entities and call the provided render function
        for (auto entity : view)
        {
            renderFunction(registry, entity);
        }
    }

    // Sets "uViewProjectionMatrix" uniform inside a given shader using a given camera component
    static void setViewProjectionMatrixUniform(Shader& shader, const CameraComponent2D& camera)
    {
        const glm::mat4& viewProjectionMatrix = camera.getViewProjectionMatrix();
        shader.setUniformMatrix4fv("uViewProjectionMatrix", viewProjectionMatrix);
    }

    // Creates a render object from given vertices and indices of a shape with solid color material
    static void createRenderObjectForShapeWithSolidColorMaterial
    (
        const entt::registry& registry,
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
            // Set view projection matrix uniform using the primary camera
            const CameraComponent2D& camera = CameraSystem2D::getPrimaryCamera(registry);
            setViewProjectionMatrixUniform(shader, camera);
        }
    }

    // Renders an entity with a shape geometry and a solid color material
    // given shape's vertices (with already filled position attributes)
    // and shape's indices
    static void renderShapeWithSolidColorMaterial
    (
        const entt::registry& registry, entt::entity entity,
        VertexOfShapeWithSolidColorMaterial* vertices,    // array of shape's vertices with already filled position attributes
        int verticesCount,                                // number of shape's vertices
        const unsigned* indices,                          // indices array
        int indicesCount                                  // number of indices
    )
    {
        // Get vertex colors into the color attribute of vertices array
        SolidColorMaterialSystem::getVertexColors
        (
            registry, entity,
            vertices, verticesCount,
            sizeof(VertexOfShapeWithSolidColorMaterial),
            offsetof(VertexOfShapeWithSolidColorMaterial, color)
        );

        // Create render object from shape's vertices and indices
        RenderObject renderObject;
        createRenderObjectForShapeWithSolidColorMaterial
        (
            registry,
            vertices, verticesCount,
            indices, indicesCount,
            renderObject
        );

        // Render shape's render object
        renderObject.render();
    }

    // Renders an entity with a shape geometry and a solid color material
    // given a function for getting shape's vertex positions
    // and given shape's indices
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
            vertices.data(),
            sizeof(VertexOfShapeWithSolidColorMaterial),
            offsetof(VertexOfShapeWithSolidColorMaterial, position)
        );

        // Render shape using the obtained vertices and given indices
        renderShapeWithSolidColorMaterial
        (
            registry, entity,
            vertices.data(), verticesCount,
            indices, indicesCount
        );
    }

    // Renders an entity with a shape geometry and a solid color material
    // given a function for getting shape's vertex positions and indices
    static void renderShapeWithSolidColorMaterial
    (
        const entt::registry& registry,
        entt::entity entity,
        VertexPositionsAndIndicesGetter vertexPositionsAndIndicesGetter,    // a function for getting shape's vertex positions and indices
        int verticesCount                                                   // number of shape's vertices
    )
    {
        // Create vertices array with given number of vertices
        std::vector<VertexOfShapeWithSolidColorMaterial> vertices(verticesCount);

        std::vector<unsigned> indices;
        // Get vertex positions into the position attribute of vertices array and get indices
        vertexPositionsAndIndicesGetter
        (
            registry, entity,
            vertices.data(), verticesCount,
            sizeof(VertexOfShapeWithSolidColorMaterial),
            offsetof(VertexOfShapeWithSolidColorMaterial, position),
            indices
        );

        // Render shape using the obtained vertices and indices
        renderShapeWithSolidColorMaterial
        (
            registry, entity,
            vertices.data(), verticesCount,
            indices.data(), int(indices.size())
        );
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
            RectangleGeometrySystem::getVertexPositions, 4,
            indices, 6
        );
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
            LineGeometrySystem::getVertexPositions, 4,
            indices, 6
        );
    }

    // Renders an entity with circle geometry and a solid color material
    static void renderCircleWithSolidColorMaterial(const entt::registry& registry, entt::entity entity)
    {
        // Get number of vertices needed for entity's circle geometry
        const int verticesCount = CircleGeometrySystem::getNumberOfVertices(registry, entity);

        // Render circle as a general shape with solid color material
        renderShapeWithSolidColorMaterial
        (
            registry, entity,
            CircleGeometrySystem::getVertexPositionsAndIndices,
            verticesCount
        );
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
            TriangleGeometrySystem::getVertexPositions, 3,
            indices, 3
        );
    }

    // Renders an entity with polygon geometry and a solid color material
    static void renderPolygonWithSolidColorMaterial(const entt::registry& registry, entt::entity entity)
    {
        // Get number of vertices needed for entity's polygon geometry
        const int verticesCount = PolygonGeometrySystem::getNumberOfVertices(registry, entity);

        // Render polygon as a general shape with solid color material
        renderShapeWithSolidColorMaterial
        (
            registry, entity,
            PolygonGeometrySystem::getVertexPositions,
            verticesCount
        );
    }

    void Renderer2DSystem_ECS::render(const entt::registry& registry)
    {
        // Render all rectangles that have a solid color material
        renderAllEntitiesWith<RectangleGeometryComponent, TransformComponent2D, SolidColorMaterialComponent>(registry, renderRectangleWithSolidColorMaterial);
        // Render all triangles that have a solid color material
        renderAllEntitiesWith<TriangleGeometryComponent, TransformComponent2D, SolidColorMaterialComponent>(registry, renderTriangleWithSolidColorMaterial);
        // Render all circles that have a solid color material
        renderAllEntitiesWith<CircleGeometryComponent, TransformComponent2D, SolidColorMaterialComponent>(registry, renderCircleWithSolidColorMaterial);
        // Render all lines that have a solid color material
        renderAllEntitiesWith<LineGeometryComponent, TransformComponent2D, SolidColorMaterialComponent>(registry, renderLineWithSolidColorMaterial);
        // Render all polygons that have a solid color material
        renderAllEntitiesWith<PolygonGeometryComponent, TransformComponent2D, SolidColorMaterialComponent>(registry, renderPolygonWithSolidColorMaterial);

        // Render all sprites
        SpriteSystem::render(registry);
    }

} // namespace Renderer2D
} // namespace Pekan
