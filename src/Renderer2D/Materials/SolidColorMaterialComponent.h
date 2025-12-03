#pragma once

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

	struct SolidColorMaterialComponent
	{
		///// Data /////
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		////////////////


		/* functions */

		// Fills the color attribute of a given array of vertices
		// with the color of this material
		void fillVerticesColorAttribute
		(
			void* vertices,             // output array of vertices
			int verticesCount,          // number of vertices in the array
			int vertexSize,             // size of a single vertex, in bytes
			int colorAttributeOffset    // offset from the start of each vertex to the color attribute, in bytes
		);
	};

} // namespace Renderer2D
} // namespace Pekan
