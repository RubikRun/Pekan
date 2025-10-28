#pragma once

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

	struct SolidColorMaterialComponent
	{
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

} // namespace Renderer2D
} // namespace Pekan
