#include "EntityPropertiesGUIWindow.h"
#include "EditorScene.h"
#include "EntitiesGUIWindow.h"

#include "PekanLogger.h"
#include "RenderCommands.h"

#include <imgui.h>
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

		return true;
	}

	void EntityPropertiesGUIWindow::_render() const
	{
		PK_ASSERT_QUICK(m_scene != nullptr);
		PK_ASSERT_QUICK(m_entitiesGUIWindow != nullptr);

		const entt::entity selectedEntity = m_entitiesGUIWindow->getSelectedEntity();

		if (selectedEntity == entt::null)
		{
			ImGui::Text("No entity selected");
			return;
		}

		ImGui::Text("Selected Entity: %u", static_cast<uint32_t>(selectedEntity));

		ImGui::Separator();
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
