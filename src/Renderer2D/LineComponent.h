#pragma once

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

	struct LineComponent
	{
		glm::vec2 pointA = { 0.0f, 0.0f };
		glm::vec2 pointB = { 0.0f, 0.0f };
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

} // namespace Renderer2D
} // namespace Pekan
