#pragma once

#include <entt/entt.hpp>
#include <vector>

namespace Pekan
{
namespace Renderer2D
{

    class CircleGeometrySystem
    {
    public:

        // Retrieves the vertex positions and indices from given entity's CircleGeometryComponent and TransformComponent2D.
        // Sets vertex positions as position attributes in the given array of vertices.
        // Fills indices to the given array of indices.
        static void getVertexPositionsAndIndices
        (
            const entt::registry& registry,
            entt::entity entity,
            void* vertices,                   // output array of vertices
            int verticesCount,                // number of vertices in the array
            int vertexSize,                   // size of a single vertex, in bytes
            int positionAttributeOffset,      // offset from the start of each vertex to the position attribute, in bytes
            std::vector<unsigned>& indices    // output array of indices
        );

        // Returns the number of vertices needed for the circle geometry of the given entity
        static int getNumberOfVertices(const entt::registry& registry, entt::entity entity);
    };

} // namespace Renderer2D
} // namespace Pekan
