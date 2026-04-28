#include "Scene.h"

#include "PekanLogger.h"

#include "Entity/DisabledComponent.h"
#include "Entity/EntityIDComponent.h"

#include <algorithm>

namespace Pekan
{

	entt::entity Scene::createEntity()
	{
		const entt::entity entity = m_registry.create();
		m_registry.emplace<EntityIDComponent>(entity, m_nextEntityId++);
		m_entities.push_back(entity);
		return entity;
	}

	entt::entity Scene::createEntity(EntityID entityId)
	{
		PK_ASSERT_QUICK(entityId != INVALID_ENTITY_ID);

		const entt::entity entity = m_registry.create();
		m_registry.emplace<EntityIDComponent>(entity, entityId);
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

	void Scene::clear()
	{
		m_registry.clear();
		m_entities.clear();
		m_nextEntityId = 1;
	}

	void Scene::adoptFrom(Scene&& other)
	{
		// Self-adoption is a no-op
		if (&other == this)
		{
			return;
		}

		m_registry = std::move(other.m_registry);
		m_entities = std::move(other.m_entities);
		m_nextEntityId = other.m_nextEntityId;

		// Leave the other scene in a well-defined empty state.
		other.m_nextEntityId = 1;
	}

	void Scene::setNextEntityId(EntityID id)
	{
		m_nextEntityId = id;
	}

} // namespace Pekan
