#include "TransformComponent2D.h"

namespace Pekan
{
namespace Renderer2D
{

	void TransformComponent2D::move(glm::vec2 deltaPosition)
	{
		position += deltaPosition;
	}

	void TransformComponent2D::rotate(float deltaRotation)
	{
		rotation += deltaRotation;
	}

	void TransformComponent2D::scale(glm::vec2 deltaScale)
	{
		scaleFactor *= deltaScale;
	}

} // namespace Renderer2D
} // namespace Pekan
