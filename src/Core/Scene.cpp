#include "Scene.h"

#include "PekanLogger.h"

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

}
