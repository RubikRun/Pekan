#include "SpriteSystem.h"

#include "TransformComponent2D.h"
#include "SpriteComponent.h"
#include "Renderer2DSystem.h"

namespace Pekan
{
namespace Renderer2D
{

	void SpriteSystem::render(const Scene& scene)
	{
	}

	void SpriteSystem::render(const entt::registry& registry, entt::entity entity)
	{
		const SpriteComponent& sprite = registry.get<SpriteComponent>(entity);
		const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);
		Renderer2DSystem::submitForRendering()
	}

} // namespace Renderer2D
} // namespace Pekan
