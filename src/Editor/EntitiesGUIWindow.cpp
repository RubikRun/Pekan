#include "EntitiesGUIWindow.h"

#include "EditorScene.h"
#include "EntityPropertiesGUIWindow.h"
#include "PekanLogger.h"

using namespace Pekan;
using namespace Pekan::GUI;

namespace Pekan
{
namespace Editor
{

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

		const std::vector<entt::entity>& entities = m_scene->getEntities();

		// Create a vector of entity labels to display in the list widget
		std::vector<std::string> entityLabels;
		for (const entt::entity entity : entities)
		{
			entityLabels.push_back("Entity " + std::to_string(static_cast<uint32_t>(entity)));
		}
		// Update entities list widget with the new entity labels
		gui.entitiesListWidget->setItems(entityLabels);
	}

	void EntitiesGUIWindow::updateEntityPropertiesGUIWindow()
	{
		PK_ASSERT_QUICK(m_entityPropertiesGuiWindow != nullptr);

		if (m_selectedEntityIndex < 0)
		{
			m_entityPropertiesGuiWindow->setEntity(entt::null);
			return;
		}

		const std::vector<entt::entity>& entities = m_scene->getEntities();
		PK_ASSERT_QUICK(m_selectedEntityIndex < int(entities.size()));

		const entt::entity selectedEntity = entities[m_selectedEntityIndex];
		m_entityPropertiesGuiWindow->setEntity(selectedEntity);
	}

} // namespace Editor
} // namespace Pekan
