#pragma once

#include <entt/entt.hpp>

namespace Pekan
{
namespace Renderer2D
{

	class RectangleGeometrySystem
    {
    public:

		// Fills the given vertices array with positions from the entity's RectangleGeometryComponent and TransformComponent2D
        static void fillVertexPositions
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
