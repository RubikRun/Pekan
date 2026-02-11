#include "EntityPropertiesGUIWindow.h"
#include "EditorScene.h"
#include "EntitiesGUIWindow.h"

#include "PekanLogger.h"
#include "RenderCommands.h"

#include <entt/entt.hpp>

using namespace Pekan;
using namespace Pekan::GUI;

namespace Pekan
{
namespace Editor
{

	bool EntityPropertiesGUIWindow::_init()
	{
		if (m_scene == nullptr)
		{
			PK_LOG_ERROR("Failed to initialize EntityPropertiesGUIWindow because there is no scene attached.", "Pekan");
			return false;
		}
		if (m_entitiesGUIWindow == nullptr)
		{
			PK_LOG_ERROR("Failed to initialize EntityPropertiesGUIWindow because there is no EntitiesGUIWindow attached.", "Pekan");
			return false;
		}

		gui.entityInfoTextWidget->create(this, "No entity selected");

		return true;
	}

	void EntityPropertiesGUIWindow::update(double deltaTime)
	{
		PK_ASSERT_QUICK(m_scene != nullptr);
		PK_ASSERT_QUICK(m_entitiesGUIWindow != nullptr);

		const entt::entity selectedEntity = m_entitiesGUIWindow->getSelectedEntity();

		// Only update text if the selected entity has changed
		if (selectedEntity != m_selectedEntity)
		{
			m_selectedEntity = selectedEntity;

			std::string entityInfoText;
			if (selectedEntity == entt::null)
			{
				entityInfoText = "No entity selected";
			}
			else
			{
				entityInfoText = "Selected Entity: " + std::to_string(static_cast<uint32_t>(selectedEntity));
			}

			gui.entityInfoTextWidget->setText(entityInfoText);
		}
	}

	GUIWindowProperties EntityPropertiesGUIWindow::getProperties() const
	{
		GUIWindowProperties props;
		props.size = { 400, 700 };
		props.name = "Entity Properties";
		return props;
	}

} // namespace Editor
} // namespace Pekan
