#include "Scene.h"

#include "PekanLogger.h"
#include "Entity/DisabledComponent.h"

namespace Pekan
{

	entt::entity Scene::createEntity()
	{
		entt::entity entity = m_registry.create();
		return entity;
	}

	void Scene::destroyEntity(entt::entity entity)
	{
		if (entity == entt::null)
		{
			PK_LOG_WARNING("Trying to destroy a null entity.", "Pekan");
			return;
		}

		m_registry.destroy(entity);
	}

	void Scene::enableEntity(entt::entity entity)
	{
		if (m_registry.all_of<DisabledComponent>(entity))
		{
			m_registry.remove<DisabledComponent>(entity);
		}
	}

	void Scene::disableEntity(entt::entity entity)
	{
		if (!m_registry.all_of<DisabledComponent>(entity))
		{
			m_registry.emplace<DisabledComponent>(entity);
		}
	}

} // namespace Pekan