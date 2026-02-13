#include "EntityPropertiesGUIWindow.h"

#include "EditorScene.h"
#include "PekanLogger.h"

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

		gui.entityInfoTextWidget->create(this, "No entity selected");
		gui.separatorWidget->create(this);
		gui.addComponentComboBoxWidget->create(this, "", 0, EditorScene::getComponentTypesNames());
		gui.addComponentButtonWidget->create(this, "Add Component");

		return true;
	}

	void EntityPropertiesGUIWindow::update(double deltaTime)
	{
		PK_ASSERT_QUICK(m_scene != nullptr);

		if (gui.addComponentButtonWidget->isClicked() && m_entity != entt::null)
		{
			const int componentTypeIndex = gui.addComponentComboBoxWidget->getIndex();
			m_scene->addComponent(m_entity, componentTypeIndex);
		}
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
