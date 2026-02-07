#pragma once

#include "TransformComponent2D.h"

#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace Pekan
{
namespace Renderer2D
{

	class TransformSystem2D
	{
	public:

		static glm::mat3 getWorldMatrix(const entt::registry& registry, entt::entity entity);
		static glm::mat3 getWorldMatrix(const entt::registry& registry, const TransformComponent2D& transform);
	};

} // namespace Renderer2D
} // namespace Pekan
