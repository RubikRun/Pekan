#pragma once

#include <entt/entt.hpp>

namespace Pekan
{
namespace Renderer2D
{

	class RectangleGeometrySystem
    {
    public:

        // Retrieves the vertex positions from given entity's RectangleGeometryComponent and TransformComponent2D,
        // and sets them as position attributes in the given array of vertices.
        static void getVertexPositions
        (
            const entt::registry& registry,
            entt::entity entity,
            void* vertices,                // output array of vertices
            int vertexSize,                // size of a single vertex, in bytes
            int positionAttributeOffset    // offset from the start of each vertex to the position attribute, in bytes
        );
    };

} // namespace Renderer2D
} // namespace Pekan
