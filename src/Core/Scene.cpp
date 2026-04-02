#include "Scene.h"

#include "PekanLogger.h"

#include <algorithm>
#include "Entity/DisabledComponent.h"

namespace Pekan
{

	entt::entity Scene::createEntity()
	{
		entt::entity entity = m_registry.create();
		m_entities.push_back(entity);
		return entity;
	}

	void Scene::destroyEntity(entt::entity entity)
	{
		if (entity == entt::null)
		{
			PK_LOG_WARNING("Trying to destroy a null entity.", "Pekan");
			return;
		}

		auto it = std::find(m_entities.begin(), m_entities.end(), entity);
		if (it != m_entities.end())
		{
			m_entities.erase(it);
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