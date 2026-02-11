#pragma once

#include "GUIWindow.h"

#include "TextWidget.h"
#include "ColorEdit3Widget.h"
#include "DragFloat2Widget.h"
#include "ComboBoxWidget.h"
#include "SeparatorWidget.h"
#include "SliderFloat2Widget.h"
#include "SliderFloatWidget.h"

#include <entt/entt.hpp>

namespace Pekan
{
namespace Editor
{

	class EditorScene;
	class EntitiesGUIWindow;

	class EntityPropertiesGUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

		EntityPropertiesGUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		std::string getLayerName() const override { return "entity_properties_gui_window"; }

		void update(double deltaTime) override;

		// Sets the scene that this GUI window will interact with
		void setScene(std::shared_ptr<EditorScene> scene) { m_scene = scene; }
		// Sets the Entities GUI window that this Entity Properties GUI window will interact with
		void setEntitiesGUIWindow(std::shared_ptr<EntitiesGUIWindow> entitiesGUIWindow) { m_entitiesGUIWindow = entitiesGUIWindow; }

	private: /* functions */

		bool _init() override;

		Pekan::GUI::GUIWindowProperties getProperties() const override;

	private: /* variables */

		struct Widgets
		{
			Pekan::GUI::TextWidget_Ptr entityInfoTextWidget = std::make_shared<Pekan::GUI::TextWidget>();
		} gui;

		std::shared_ptr<EditorScene> m_scene;
		std::shared_ptr<EntitiesGUIWindow> m_entitiesGUIWindow;

		// Currently selected entity
		entt::entity m_selectedEntity = entt::null;
	};

} // namespace Editor
} // namespace Pekan