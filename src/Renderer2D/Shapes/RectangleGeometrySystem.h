#pragma once

#include <entt/entt.hpp>

namespace Pekan
{
namespace Renderer2D
{

	class RectangleGeometrySystem
    {
    public:

        static void getVertexPositions
        (
            const entt::registry& registry,
            entt::entity entity,
            void* vertices,              // output array of vertices
            int vertexSize,              // size of a single vertex, in bytes
            int offsetFromVertexStart    // offset from the start of each vertex to the position data, in bytes
        );
    };

} // namespace Renderer2D
} // namespace Pekan
