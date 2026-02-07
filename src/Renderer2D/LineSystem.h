#pragma once

#include <entt/entt.hpp>

namespace Pekan
{
namespace Renderer2D
{

	class LineSystem
	{
	public:

		static void getVertexPositionsLocal
		(
			const entt::registry& registry,
			entt::entity entity,
			void* vertices,                // output array of vertices
			int vertexSize,                // size of a single vertex, in bytes
			int positionAttributeOffset    // offset from the start of each vertex to the position attribute, in bytes
		);

		static void getVertexPositionsWorld
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
