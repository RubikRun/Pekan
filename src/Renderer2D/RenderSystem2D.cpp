#include "RenderSystem2D.h"

#include "TransformComponent2D.h"
#include "SpriteSystem.h"

#include "DrawObject.h"
#include "CameraComponent2D.h"
#include "CameraSystem2D.h"
#include "Entity/DisabledComponent.h"

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

#include "LineComponent.h"
#include "LineSystem.h"

////////// Material components and systems //////////
#include "SolidColorMaterialComponent.h"
/////////////////////////////////////////////////////

////////// Pekan Core includes //////////
#include "Utils/FileUtils.h"
#include "PekanLogger.h"
/////////////////////////////////////////

using namespace Pekan::Graphics;

#define SHAPE_WITH_SOLID_COLOR_MATERIAL_VERTEX_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shaders/2D_Shape_SolidColorMaterial_VertexShader.glsl"
#define SHAPE_WITH_SOLID_COLOR_MATERIAL_FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shaders/2D_Shape_SolidColorMaterial_FragmentShader.glsl"

#define LINE_VERTEX_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shaders/2D_Line_VertexShader.glsl"
#define LINE_FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shaders/2D_Line_SolidColor_FragmentShader.glsl"

namespace Pekan
{
namespace Renderer2D
{

    // Current primary camera cached here for easy access
    static const CameraComponent2D* g_camera = nullptr;

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

    // Type alias for a vertex of a line
    typedef glm::vec2 VertexOfLine;

    // Renders all entities with given component types (except those with DisabledComponent)
    // by calling a given render function for each entity
    //
    // @tparam ComponentTypes...  Component types that an entity must have to be rendered
    // @param[in] registry        Registry containing entities to render
    // @param[in] renderFunction  Function that renders a single entity
    template<typename... ComponentTypes>
    void renderAllEntitiesWith(const entt::registry& registry, RenderFunction renderFunction)
    {
        // Create a view over all entities that have the given components
        const auto view = registry.view<ComponentTypes...>(entt::exclude<DisabledComponent>);
        // Iterate over entities and call the provided render function
        for (auto entity : view)
        {
            renderFunction(registry, entity);
        }
    }

    // Renders all entities that have all components from ComponentTypesToInclude
    // and do NOT have any components from ComponentTypesToExclude (except those with DisabledComponent),
    // by calling a given render function for each entity
    //
    // @tparam ComponentTypesToInclude...  Component types that an entity must have to be rendered
    // @tparam ComponentTypesToExclude...  Component types that an entity must NOT have to be rendered
    // @param[in] registry        Registry containing entities to render
    // @param[in] renderFunction  Function that renders a single entity
    template<typename... ComponentTypesToInclude, typename... ComponentTypesToExclude>
    void renderAllEntitiesWith
    (
        entt::exclude_t<ComponentTypesToExclude...>,
        const entt::registry& registry,
        RenderFunction renderFunction
    )
    {
        // Create a view over all entities that
        // have the given components to include and do NOT have the given components to exclude
        const auto view = registry.view<ComponentTypesToInclude...>(entt::exclude<DisabledComponent, ComponentTypesToExclude...>);
        // Iterate over entities and call the provided render function
        for (auto entity : view)
        {
            renderFunction(registry, entity);
        }
    }

    // Sets "uViewProjectionMatrix" uniform inside a given shader using a given camera component
    static void setViewProjectionMatrixUniform(Shader& shader, const CameraComponent2D* camera)
    {
        PK_ASSERT(camera != nullptr, "Cannot set view projection matrix uniform without a camera.", "Pekan");
        const glm::mat4& viewProjectionMatrix = camera->getViewProjectionMatrix();
        shader.setUniformMatrix4fv("uViewProjectionMatrix", viewProjectionMatrix);
    }

    // Creates a draw object from given vertices and indices of a shape with solid color material
    static void createDrawObjectForShapeWithSolidColorMaterial
    (
        const entt::registry& registry,
        const VertexOfShapeWithSolidColorMaterial* vertices, int verticesCount,
        const unsigned *indices, int indicesCount,
        DrawObject& drawObject    // draw object to create
    )
    {
        // Create draw object with given vertices
        drawObject.create
        (
            vertices,
            sizeof(VertexOfShapeWithSolidColorMaterial) * verticesCount,
            {
                { ShaderDataType::Float2, "position" },
                { ShaderDataType::Float4, "color" }
            },
            BufferDataUsage::StaticDraw,
            FileUtils::readTextFileToString(SHAPE_WITH_SOLID_COLOR_MATERIAL_VERTEX_SHADER_FILEPATH).c_str(),
            FileUtils::readTextFileToString(SHAPE_WITH_SOLID_COLOR_MATERIAL_FRAGMENT_SHADER_FILEPATH).c_str()
        );
        // Set given indices to the draw object
        drawObject.setIndexData(indices, sizeof(unsigned) * indicesCount);

        // Set draw object's shader uniforms
        {
            Shader& shader = drawObject.getShader();
            // Set view projection matrix uniform using the primary camera
            setViewProjectionMatrixUniform(shader, g_camera);
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
        // Get material component from entity
        PK_ASSERT(registry.all_of<SolidColorMaterialComponent>(entity), "Cannot get vertex colors of an entity that doesn't have a SolidColorMaterialComponent.", "Pekan");
        const SolidColorMaterialComponent& material = registry.get<SolidColorMaterialComponent>(entity);

        // Get vertex colors using material
        material.getVertexColors(
            vertices, verticesCount,
            sizeof(VertexOfShapeWithSolidColorMaterial),
            offsetof(VertexOfShapeWithSolidColorMaterial, color)
        );

        // Create draw object from shape's vertices and indices
        DrawObject drawObject;
        createDrawObjectForShapeWithSolidColorMaterial
        (
            registry,
            vertices, verticesCount,
            indices, indicesCount,
            drawObject
        );

        // Render shape's draw object
        drawObject.render();
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
    // @tparam HasTransform - A boolean parameter indicating if the entity has a transform component
    template<bool HasTransform>
    static void renderRectangleWithSolidColorMaterial(const entt::registry& registry, entt::entity entity);

    template<>
    static void renderRectangleWithSolidColorMaterial<true>(const entt::registry& registry, entt::entity entity)
    {
        // Define indices for two triangles making up a rectangle
        static constexpr unsigned indices[6] = { 0, 1, 2, 0, 2, 3 };

        // Render rectangle as a general shape with solid color material
        renderShapeWithSolidColorMaterial
        (
            registry, entity,
            RectangleGeometrySystem::getVertexPositionsWorld, 4,
            indices, 6
        );
    }

    template<>
    static void renderRectangleWithSolidColorMaterial<false>(const entt::registry& registry, entt::entity entity)
    {
        // Define indices for two triangles making up a rectangle
        static constexpr unsigned indices[6] = { 0, 1, 2, 0, 2, 3 };

        // Render rectangle as a general shape with solid color material
        renderShapeWithSolidColorMaterial
        (
            registry, entity,
            RectangleGeometrySystem::getVertexPositionsLocal, 4,
            indices, 6
        );
    }

    // Renders an entity with line geometry and a solid color material
    // @tparam HasTransform - A boolean parameter indicating if the entity has a transform component
    template<bool HasTransform>
    static void renderLineWithSolidColorMaterial(const entt::registry& registry, entt::entity entity);

    template<>
    static void renderLineWithSolidColorMaterial<true>(const entt::registry& registry, entt::entity entity)
    {
        // Define indices for two triangles making up a rectangle
        // (since lines are rendered as thin rectangles)
        static constexpr unsigned indices[6] = { 0, 1, 2, 0, 2, 3 };

        // Render line as a general shape with solid color material
        renderShapeWithSolidColorMaterial
        (
            registry, entity,
            LineGeometrySystem::getVertexPositionsWorld, 4,
            indices, 6
        );
    }

    template<>
    static void renderLineWithSolidColorMaterial<false>(const entt::registry& registry, entt::entity entity)
    {
        // Define indices for two triangles making up a rectangle
        // (since lines are rendered as thin rectangles)
        static constexpr unsigned indices[6] = { 0, 1, 2, 0, 2, 3 };

        // Render line as a general shape with solid color material
        renderShapeWithSolidColorMaterial
        (
            registry, entity,
            LineGeometrySystem::getVertexPositionsLocal, 4,
            indices, 6
        );
    }

    // Renders an entity with circle geometry and a solid color material
    // @tparam HasTransform - A boolean parameter indicating if the entity has a transform component
    template<bool HasTransform>
    static void renderCircleWithSolidColorMaterial(const entt::registry& registry, entt::entity entity);

    template<>
    static void renderCircleWithSolidColorMaterial<true>(const entt::registry& registry, entt::entity entity)
    {
        const CircleGeometryComponent& circleGeometry = registry.get<CircleGeometryComponent>(entity);

        // Render circle as a general shape with solid color material
        renderShapeWithSolidColorMaterial
        (
            registry, entity,
            CircleGeometrySystem::getVertexPositionsAndIndicesWorld,
            circleGeometry.segmentsCount    // number of vertices is equal to number of segments
        );
    }

    template<>
    static void renderCircleWithSolidColorMaterial<false>(const entt::registry& registry, entt::entity entity)
    {
        const CircleGeometryComponent& circleGeometry = registry.get<CircleGeometryComponent>(entity);

        // Render circle as a general shape with solid color material
        renderShapeWithSolidColorMaterial
        (
            registry, entity,
            CircleGeometrySystem::getVertexPositionsAndIndicesLocal,
            circleGeometry.segmentsCount    // number of vertices is equal to number of segments
        );
    }

    // Renders an entity with triangle geometry and a solid color material
    // @tparam HasTransform - A boolean parameter indicating if the entity has a transform component
    template<bool HasTransform>
    static void renderTriangleWithSolidColorMaterial(const entt::registry& registry, entt::entity entity);

    template<>
    static void renderTriangleWithSolidColorMaterial<true>(const entt::registry& registry, entt::entity entity)
    {
        // Define indices for a single triangle
        static constexpr unsigned indices[3] = { 0, 1, 2 };

        // Render triangle as a general shape with solid color material
        renderShapeWithSolidColorMaterial
        (
            registry, entity,
            TriangleGeometrySystem::getVertexPositionsWorld, 3,
            indices, 3
        );
    }

    template<>
    static void renderTriangleWithSolidColorMaterial<false>(const entt::registry& registry, entt::entity entity)
    {
        // Define indices for a single triangle
        static constexpr unsigned indices[3] = { 0, 1, 2 };

        // Render triangle as a general shape with solid color material
        renderShapeWithSolidColorMaterial
        (
            registry, entity,
            TriangleGeometrySystem::getVertexPositionsLocal, 3,
            indices, 3
        );
    }

    // Renders an entity with polygon geometry and a solid color material
    // @tparam HasTransform - A boolean parameter indicating if the entity has a transform component
    template<bool HasTransform>
    static void renderPolygonWithSolidColorMaterial(const entt::registry& registry, entt::entity entity);

    template<>
    static void renderPolygonWithSolidColorMaterial<true>(const entt::registry& registry, entt::entity entity)
    {
        const PolygonGeometryComponent& polygonGeometry = registry.get<PolygonGeometryComponent>(entity);

        // Render polygon as a general shape with solid color material
        renderShapeWithSolidColorMaterial
        (
            registry, entity,
            PolygonGeometrySystem::getVertexPositionsAndIndicesWorld,
            polygonGeometry.vertexPositions.size()
        );
    }

    template<>
    static void renderPolygonWithSolidColorMaterial<false>(const entt::registry& registry, entt::entity entity)
    {
        const PolygonGeometryComponent& polygonGeometry = registry.get<PolygonGeometryComponent>(entity);

        // Render polygon as a general shape with solid color material
        renderShapeWithSolidColorMaterial
        (
            registry, entity,
            PolygonGeometrySystem::getVertexPositionsAndIndicesLocal,
            polygonGeometry.vertexPositions.size()
        );
    }

    // Renders an entity with a line component
    // @tparam HasTransform - A boolean parameter indicating if the entity has a transform component
    template<bool HasTransform>
    static void renderLine(const entt::registry& registry, entt::entity entity);

    template<>
    static void renderLine<true>(const entt::registry& registry, entt::entity entity)
    {
        // Get line component from entity
        const LineComponent& line = registry.get<LineComponent>(entity);

        // Create an array for line's 2 vertices
        VertexOfLine vertices[2];
        // Get vertex positions into the vertices array
        LineSystem::getVertexPositionsWorld(registry, entity, vertices, sizeof(VertexOfLine), 0);

        // Create draw object with line's vertices
        DrawObject drawObject;
        drawObject.create
        (
            vertices, 2 * sizeof(VertexOfLine),
            { { ShaderDataType::Float2, "position" } },
            BufferDataUsage::StaticDraw,
            FileUtils::readTextFileToString(LINE_VERTEX_SHADER_FILEPATH).c_str(),
            FileUtils::readTextFileToString(LINE_FRAGMENT_SHADER_FILEPATH).c_str()
        );

        // Set draw object's shader uniforms
        {
            Shader& shader = drawObject.getShader();
            // Set "uColor" uniform to line's color
            shader.setUniform4f("uColor", line.color);
            // Set view projection matrix uniform using the primary camera
            setViewProjectionMatrixUniform(shader, g_camera);
        }

        // Render the draw object
        drawObject.render(DrawMode::Lines);
    }

    template<>
    static void renderLine<false>(const entt::registry& registry, entt::entity entity)
    {
        // Get line component from entity
        const LineComponent& line = registry.get<LineComponent>(entity);

        // Create an array for line's 2 vertices
        VertexOfLine vertices[2];
        // Get vertex positions into the vertices array
        LineSystem::getVertexPositionsLocal(registry, entity, vertices, sizeof(VertexOfLine), 0);

        // Create draw object with line's vertices
        DrawObject drawObject;
        drawObject.create
        (
            vertices, 2 * sizeof(VertexOfLine),
            { { ShaderDataType::Float2, "position" } },
            BufferDataUsage::StaticDraw,
            FileUtils::readTextFileToString(LINE_VERTEX_SHADER_FILEPATH).c_str(),
            FileUtils::readTextFileToString(LINE_FRAGMENT_SHADER_FILEPATH).c_str()
        );

        // Set draw object's shader uniforms
        {
            Shader& shader = drawObject.getShader();
            // Set "uColor" uniform to line's color
            shader.setUniform4f("uColor", line.color);
            // Set view projection matrix uniform using the primary camera
            setViewProjectionMatrixUniform(shader, g_camera);
        }

        // Render the draw object
        drawObject.render(DrawMode::Lines);
    }

    void RenderSystem2D::render(const entt::registry& registry)
    {
        // Update cached camera with current primary camera
        g_camera = &CameraSystem2D::getPrimaryCamera(registry);
        if (g_camera == nullptr)
        {
            PK_LOG_ERROR("No primary camera found in the registry. Cannot render the scene without a camera.", "Pekan");
            return;
        }

        // Render all rectangles, triangles, circles, lines, and polygons that have a solid color material and a transform
        renderAllEntitiesWith<RectangleGeometryComponent, SolidColorMaterialComponent, TransformComponent2D>(registry, renderRectangleWithSolidColorMaterial<true>);
        renderAllEntitiesWith<TriangleGeometryComponent, SolidColorMaterialComponent, TransformComponent2D>(registry, renderTriangleWithSolidColorMaterial<true>);
        renderAllEntitiesWith<CircleGeometryComponent, SolidColorMaterialComponent, TransformComponent2D>(registry, renderCircleWithSolidColorMaterial<true>);
        renderAllEntitiesWith<LineGeometryComponent, SolidColorMaterialComponent, TransformComponent2D>(registry, renderLineWithSolidColorMaterial<true>);
        renderAllEntitiesWith<PolygonGeometryComponent, SolidColorMaterialComponent, TransformComponent2D>(registry, renderPolygonWithSolidColorMaterial<true>);
        // Render all rectangles, triangles, circles, lines, and polygons that have a solid color material but no transform
        renderAllEntitiesWith<RectangleGeometryComponent, SolidColorMaterialComponent>(entt::exclude<TransformComponent2D>, registry, renderRectangleWithSolidColorMaterial<false>);
        renderAllEntitiesWith<TriangleGeometryComponent, SolidColorMaterialComponent>(entt::exclude<TransformComponent2D>, registry, renderTriangleWithSolidColorMaterial<false>);
        renderAllEntitiesWith<CircleGeometryComponent, SolidColorMaterialComponent>(entt::exclude<TransformComponent2D>, registry, renderCircleWithSolidColorMaterial<false>);
        renderAllEntitiesWith<LineGeometryComponent, SolidColorMaterialComponent>(entt::exclude<TransformComponent2D>, registry, renderLineWithSolidColorMaterial<false>);
        renderAllEntitiesWith<PolygonGeometryComponent, SolidColorMaterialComponent>(entt::exclude<TransformComponent2D>, registry, renderPolygonWithSolidColorMaterial<false>);

        // Render all lines that have a transform
        renderAllEntitiesWith<LineComponent, TransformComponent2D>(registry, renderLine<true>);
        // Render all lines that do not have a transform
        renderAllEntitiesWith<LineComponent>(entt::exclude<TransformComponent2D>, registry, renderLine<false>);

        // Render all sprites
        SpriteSystem::render(registry, g_camera);
    }

} // namespace Renderer2D
} // namespace Pekan
