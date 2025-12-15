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

        static void getVertexPositionsAndIndicesLocal
        (
            const entt::registry& registry,
            entt::entity entity,
            void* vertices,                   // output array of vertices
            int verticesCount,                // number of vertices in the array (equal to number of segments in the circle)
            int vertexSize,                   // size of a single vertex, in bytes
            int positionAttributeOffset,      // offset from the start of each vertex to the position attribute, in bytes
            std::vector<unsigned>& indices    // output array of indices
        );

        static void getVertexPositionsAndIndicesWorld
        (
            const entt::registry& registry,
            entt::entity entity,
            void* vertices,                   // output array of vertices
            int verticesCount,                // number of vertices in the array (equal to number of segments in the circle)
            int vertexSize,                   // size of a single vertex, in bytes
            int positionAttributeOffset,      // offset from the start of each vertex to the position attribute, in bytes
            std::vector<unsigned>& indices    // output array of indices
        );
    };

} // namespace Renderer2D
} // namespace Pekan
