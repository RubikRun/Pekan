#pragma once

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

	struct SpriteVertex
	{
		// Position of the vertex, in world space
		glm::vec2 position = { -1.0f, -1.0f };
		// Coordinates in texture space that this vertex maps to
		glm::vec2 textureCoordinates = { -1.0f, -1.0f };
	};

} // namespace Renderer2D
} // namespace Pekan
