#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Pekan
{
namespace Renderer2D
{

	struct PolygonGeometryComponent
	{
		std::vector<glm::vec2> vertexPositions;
	};

} // namespace Renderer2D
} // namespace Pekan
