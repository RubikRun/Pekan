#pragma once

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

	struct SolidColorMaterialComponent
	{
	/* data */

		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	/* functions */

		// Retrieves the vertex color from this material,
		// and sets it as the color attribute of each vertex in the given array of vertices.
		void getVertexColors
		(
			void* vertices,             // output array of vertices
			int verticesCount,          // number of vertices in the array
			int vertexSize,             // size of a single vertex, in bytes
			int colorAttributeOffset    // offset from the start of each vertex to the color attribute, in bytes
		) const;
	};

} // namespace Renderer2D
} // namespace Pekan
