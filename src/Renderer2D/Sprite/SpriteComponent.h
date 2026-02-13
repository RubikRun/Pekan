#pragma once

#include "Texture2D.h"

namespace Pekan
{
namespace Renderer2D
{

	struct SpriteComponent
	{
		float width = 1.0f;
		float height = 1.0f;

		Graphics::Texture2D_ConstPtr texture;

		glm::vec2 textureCoordinatesMin = { 0.0f, 0.0f };
		glm::vec2 textureCoordinatesMax = { 1.0f, 1.0f };
	};

} // namespace Renderer2D
} // namespace Pekan
