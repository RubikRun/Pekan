#pragma once

#include "GUIWindow.h"

#include "ButtonWidget.h"
#include "SelectableListWidget.h"

#include <entt/entt.hpp>

namespace Pekan
{
namespace Editor
{

	class EditorScene;
	class EntityPropertiesGUIWindow;

	class EntitiesGUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

		EntitiesGUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		std::string getLayerName() const override { return "entities_gui_window"; }

		void update(double deltaTime) override;

		// Sets the scene whose entities will be displayed in this GUI window
		void setScene(std::shared_ptr<EditorScene> scene) { m_scene = scene; }

		// Sets the "entity properties" GUI window that will show the properties of the entity selected in this GUI window
		void setEntityPropertiesGUIWindow(std::shared_ptr<EntityPropertiesGUIWindow> entityPropertiesGuiWindow) { m_entityPropertiesGuiWindow = entityPropertiesGuiWindow; }

		// Returns the index of the currently selected entity, or -1 if no entity is selected
		int getSelectedEntityIndex() const { return gui.entitiesListWidget->getSelectedIndex(); }

	private: /* functions */

		bool _init() override;

		Pekan::GUI::GUIWindowProperties getProperties() const override;

		// Updates the list widget displaying the entities with the current entities in the scene
		void updateEntitiesListWidget();

		// Updates the "entity properties" GUI window with the currently selected entity
		void updateEntityPropertiesGUIWindow();

	private: /* variables */

		struct Widgets
		{
			Pekan::GUI::ButtonWidget_Ptr addEntityButtonWidget = std::make_shared<Pekan::GUI::ButtonWidget>();
			Pekan::GUI::SelectableListWidget_Ptr entitiesListWidget = std::make_shared<Pekan::GUI::SelectableListWidget>();
		} gui;

		// Pointer to the scene whose entities are displayed in this GUI window
		std::shared_ptr<EditorScene> m_scene;

		// Pointer to the "entity properties" GUI window
		std::shared_ptr<class EntityPropertiesGUIWindow> m_entityPropertiesGuiWindow;

		// Index of the currently selected entity, or -1 if no entity is selected
		int m_selectedEntityIndex = -1;
	};

} // namespace Editor
} // namespace Pekan