#pragma once

namespace Pekan
{
namespace Renderer2D
{

	// A vertex of a 2D primitive
	struct Vertex2D
	{
		// Position of the vertex, in world space
		glm::vec2 position = { -1.0f, -1.0f };
		// Coordinates in texture space that this vertex maps to, if applicable
		glm::vec2 textureCoordinates = { -1.0f, -1.0f };
		// Index of the texture to be used for this vertex, if applicable
		float textureIndex = -1.0f;
#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		// Index of the shape that this vertex belongs to, if applicable
		float shapeIndex = -1.0f;
#else
		// Color of this vertex
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
#endif
	};

} // namespace Renderer2D
} // namespace Pekan