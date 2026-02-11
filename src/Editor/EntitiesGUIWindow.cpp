#include "EntitiesGUIWindow.h"
#include "EditorScene.h"

#include "PekanLogger.h"
#include "RenderCommands.h"

// TEMP: include imgui.h because we need to build GUI with raw ImGui temporarily until we create necessary wrappers.
#include <imgui.h>

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
		}
	}

	bool EntitiesGUIWindow::_init()
	{
		if (m_scene == nullptr)
		{
			PK_LOG_ERROR("Failed to initialize EntitiesGUIWindow because there is no scene attached.", "Pekan");
			return false;
		}

		gui.addEntityButtonWidget->create(this, "+");
		return true;
	}

	void EntitiesGUIWindow::_render() const
	{
		PK_ASSERT_QUICK(m_scene != nullptr);

		// Render a separator between the "+" button and the list of entities
		ImGui::Separator();

		// Get list of entities from the scene
		const std::vector<entt::entity>& entities = m_scene->getEntities();

		// Display list of entities
		for (const entt::entity entity : entities)
		{
			const std::string entityLabel = "Entity " + std::to_string(static_cast<uint32_t>(entity));

			// Check if this entity is the currently selected one, so that we can render it as selected/unselected
			const bool isSelected = (entity == m_selectedEntity);

			// Render selectable widget and check if it's clicked in current frame
			if (ImGui::Selectable(entityLabel.c_str(), isSelected))
			{
				// If widget is clicked, set this entity as the currently selected one
				m_selectedEntity = entity;
			}
		}
	}

	GUIWindowProperties EntitiesGUIWindow::getProperties() const
	{
		GUIWindowProperties props;
		props.size = { 400, 700 };
		props.name = "Entities";
		return props;
	}

} // namespace Editor
} // namespace Pekan
