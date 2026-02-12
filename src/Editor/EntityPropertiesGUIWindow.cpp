#include "EntityPropertiesGUIWindow.h"

using namespace Pekan;
using namespace Pekan::GUI;

namespace Pekan
{
namespace Editor
{

	bool EntityPropertiesGUIWindow::_init()
	{
		gui.entityInfoTextWidget->create(this, "No entity selected");

		return true;
	}

	void EntityPropertiesGUIWindow::setEntity(entt::entity entity)
	{
		m_entity = entity;

		std::string entityInfoText;
		if (entity == entt::null)
		{
			entityInfoText = "No entity selected";
		}
		else
		{
			entityInfoText = "Selected Entity: " + std::to_string(uint32_t(entity));
		}
		gui.entityInfoTextWidget->setText(entityInfoText);
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
