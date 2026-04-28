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
		// TODO: replace with a helper function that checks if an EntityID exists in the scene.
		PK_DEBUG_CODE
		(
			// Check if the EntityID already exists in the entity list
			for (const entt::entity entity : m_entities)
			{
				PK_ASSERT(m_registry.get<EntityIDComponent>(entity).id != entityId,
					"Trying to create entity with Entity ID " + std::to_string(entityId)
					+ " which already exists in the scene.", "Pekan");
			}
			// Check if the EntityID already exists in the registry.
			// All entities in the registry are supposed to be in the entity list as well,
			// but still, do it as a sanity check.
			for (const entt::entity entity : m_registry.view<EntityIDComponent>())
			{
				PK_ASSERT(m_registry.get<EntityIDComponent>(entity).id != entityId,
					"Trying to create entity with Entity ID " + std::to_string(entityId)
					+ " which already exists in the scene.", "Pekan");
			}
		);

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

		// Clear the other scene, leaving it in a well-defined empty state.
		other.clear();
	}

	void Scene::setNextEntityId(EntityID nextEntityId)
	{
		PK_ASSERT_QUICK(nextEntityId != INVALID_ENTITY_ID);
		// TODO: replace with a helper function that checks if an EntityID exists in the scene.
		PK_DEBUG_CODE
		(
			// Check if the given "next Entity ID" already exists in the entity list
			for (const entt::entity entity : m_entities)
			{
				PK_ASSERT(m_registry.get<EntityIDComponent>(entity).id != nextEntityId,
					"Trying to set Scene's \"next Entity ID\" to " + std::to_string(nextEntityId)
					+ " which already exists in the scene.", "Pekan");
			}
			// Check if the given "next Entity ID" already exists in the registry.
			// All entities in the registry are supposed to be in the entity list as well,
			// but still, do it as a sanity check.
			for (const entt::entity entity : m_registry.view<EntityIDComponent>())
			{
				PK_ASSERT(m_registry.get<EntityIDComponent>(entity).id != nextEntityId,
					"Trying to set Scene's \"next Entity ID\" to " + std::to_string(nextEntityId)
					+ " which already exists in the scene.", "Pekan");
			}
		);
		m_nextEntityId = nextEntityId;
	}

} // namespace Pekan
