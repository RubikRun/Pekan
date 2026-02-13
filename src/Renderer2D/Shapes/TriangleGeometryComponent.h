#pragma once

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

	struct TriangleGeometryComponent
	{
		glm::vec2 pointA = { -1.0f, 0.0f };
		glm::vec2 pointB = { 1.0f, 0.0f };
		glm::vec2 pointC = { 0.0f, 1.0f };
	};

} // namespace Renderer2D
} // namespace Pekan
