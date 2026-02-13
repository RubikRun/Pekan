#pragma once

namespace Pekan
{
namespace Renderer2D
{

	struct CircleGeometryComponent
	{
		float radius = 1.0f;
		int segmentsCount = 32;    // number of segments used to approximate the circle
	};

} // namespace Renderer2D
} // namespace Pekan
