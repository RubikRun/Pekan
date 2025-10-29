#pragma once

#include <entt/entt.hpp>

namespace Pekan
{
namespace Renderer2D
{

	class CircleGeometrySystem
    {
    public:

		// Retrieves the vertex positions from given entity's CircleGeometryComponent and TransformComponent2D
		// and sets them on the position attribute of each vertex in the given vertices array.
        static void getVertexPositions
        (
            const entt::registry& registry,
            entt::entity entity,
            void* vertices,                // output array of vertices
			int verticesCount,             // number of vertices in the array
            int vertexSize,                // size of a single vertex, in bytes
            int positionAttributeOffset    // offset from the start of each vertex to the position attribute, in bytes
        );

		// Returns the number of vertices needed for the circle geometry of the given entity
		static int getNumberOfVertices(const entt::registry& registry, entt::entity entity);
    };

} // namespace Renderer2D
} // namespace Pekan
