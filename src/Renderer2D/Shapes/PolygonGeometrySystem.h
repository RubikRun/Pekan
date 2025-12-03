#pragma once

#include <entt/entt.hpp>

namespace Pekan
{
namespace Renderer2D
{

    class PolygonGeometrySystem
    {
    public:

        // Retrieves the vertex positions and indices from given entity's PolygonGeometryComponent and TransformComponent2D.
        // Sets vertex positions as position attributes in the given array of vertices.
        // Fills indices to the given array of indices.
        static void getVertexPositions
        (
            const entt::registry& registry,
            entt::entity entity,
            void* vertices,                   // output array of vertices
            int verticesCount,                // number of vertices in the array
            int vertexSize,                   // size of a single vertex, in bytes
            int positionAttributeOffset,      // offset from the start of each vertex to the position attribute, in bytes
            std::vector<unsigned>& indices    // output array of indices
        );
    };

} // namespace Renderer2D
} // namespace Pekan
