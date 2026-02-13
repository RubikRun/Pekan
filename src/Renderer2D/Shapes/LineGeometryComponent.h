#pragma once

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

	struct LineGeometryComponent
	{
		glm::vec2 pointA = { 0.0f, 0.0f };
		glm::vec2 pointB = { 1.0f, 1.0f };
		float thickness = 0.02f;
	};

} // namespace Renderer2D
} // namespace Pekan
