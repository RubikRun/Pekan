#include "EntitiesGUIWindow.h"

#include "EditorScene.h"
#include "EntityPropertiesGUIWindow.h"
#include "PekanLogger.h"

#include "Entity/EntityIDComponent.h"
#include "Entity/NameComponent.h"

using namespace Pekan;
using namespace Pekan::GUI;

namespace Pekan
{
namespace Editor
{

	bool EntitiesGUIWindow::_init()
	{
		if (m_scene == nullptr)
		{
			PK_LOG_ERROR("Failed to initialize EntitiesGUIWindow because there is no scene attached.", "Pekan");
			return false;
		}
		if (m_entityPropertiesGuiWindow == nullptr)
		{
			PK_LOG_ERROR("Failed to initialize EntitiesGUIWindow because there is no EntityPropertiesGUIWindow attached.", "Pekan");
			return false;
		}

		gui.addEntityButtonWidget->create(this, "+");
		gui.entitiesListWidget->create(this);
		updateEntitiesListWidget();
		return true;
	}

	void EntitiesGUIWindow::update(double deltaTime)
	{
		PK_ASSERT_QUICK(m_scene != nullptr);

		if (gui.addEntityButtonWidget->isClicked())
		{
			m_scene->addEntity();
			updateEntitiesListWidget();
		}

		// If selected entity has changed, update the "entity properties" GUI window with the new selected entity
		const int selectedEntityIndex = gui.entitiesListWidget->getSelectedIndex();
		if (selectedEntityIndex != m_selectedEntityIndex)
		{
			m_selectedEntityIndex = selectedEntityIndex;
			updateEntityPropertiesGUIWindow();
		}
	}

	GUIWindowProperties EntitiesGUIWindow::getProperties() const
	{
		GUIWindowProperties props;
		props.size = { 400, 700 };
		props.name = "Entities";
		return props;
	}

	void EntitiesGUIWindow::updateEntitiesListWidget()
	{
		PK_ASSERT_QUICK(m_scene != nullptr);

		const entt::registry& registry = m_scene->getRegistry();
		const std::vector<entt::entity>& entities = m_scene->getEntities();

		// Each list item is built from the entity's stable EntityID
		// and (when present) its NameComponent, so that:
		//   - named entities show up as "<name> (<id>)", e.g. "red_rectangle (3)"
		//   - unnamed entities show up as "Entity <id>", e.g. "Entity 3"
		std::vector<std::string> entityLabels;
		entityLabels.reserve(entities.size());
		for (const entt::entity entity : entities)
		{
			PK_ASSERT_QUICK(registry.all_of<EntityIDComponent>(entity));
			const EntityID id = registry.get<EntityIDComponent>(entity).id;

			// An entity may or may not have a NameComponent, so we use try_get here
			const NameComponent* nameComponent = registry.try_get<NameComponent>(entity);
			const bool hasName = (nameComponent != nullptr) && !nameComponent->name.empty();

			if (hasName)
			{
				entityLabels.push_back(nameComponent->name + " (" + std::to_string(id) + ")");
			}
			else
			{
				entityLabels.push_back("Entity " + std::to_string(id));
			}
		}
		// Update entities list widget with the new entity labels
		gui.entitiesListWidget->setItems(entityLabels);
	}

	void EntitiesGUIWindow::updateEntityPropertiesGUIWindow()
	{
		PK_ASSERT_QUICK(m_entityPropertiesGuiWindow != nullptr);

		if (m_selectedEntityIndex < 0)
		{
			m_entityPropertiesGuiWindow->setEntity(INVALID_ENTITY_ID);
			return;
		}

		const std::vector<entt::entity>& entities = m_scene->getEntities();
		PK_ASSERT_QUICK(m_selectedEntityIndex < int(entities.size()));

		// Translate the entt::entity of the selected entity to an EntityID
		const entt::entity selectedEntity = entities[m_selectedEntityIndex];
		const entt::registry& registry = m_scene->getRegistry();
		PK_ASSERT_QUICK(registry.all_of<EntityIDComponent>(selectedEntity));
		const EntityID selectedEntityId = registry.get<EntityIDComponent>(selectedEntity).id;
		m_entityPropertiesGuiWindow->setEntity(selectedEntityId);
	}

} // namespace Editor
} // namespace Pekan
