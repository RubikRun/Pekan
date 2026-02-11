#pragma once

#include "GUIWindow.h"

#include "TextWidget.h"
#include "ColorEdit3Widget.h"
#include "DragFloat2Widget.h"
#include "ComboBoxWidget.h"
#include "SeparatorWidget.h"
#include "SliderFloat2Widget.h"
#include "SliderFloatWidget.h"
#include "ButtonWidget.h"

#include <entt/entt.hpp>

namespace Pekan
{
namespace Editor
{

	class EditorScene;

	class EntitiesGUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

		EntitiesGUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		std::string getLayerName() const override { return "entities_gui_window"; }

		void update(double deltaTime) override;

		// Sets the scene that this GUI window will interact with
		void setScene(std::shared_ptr<EditorScene> scene) { m_scene = scene; }

		// Returns the currently selected entity, or entt::null if no entity is selected
		entt::entity getSelectedEntity() const { return m_selectedEntity; }

	private: /* functions */

		bool _init() override;
		void _render() const override;

		Pekan::GUI::GUIWindowProperties getProperties() const override;

	private: /* variables */

		struct Widgets
		{
			Pekan::GUI::ButtonWidget_Ptr addEntityButtonWidget = std::make_shared<Pekan::GUI::ButtonWidget>();
		} gui;

		std::shared_ptr<EditorScene> m_scene;

		// Currently selected entity
		mutable entt::entity m_selectedEntity = entt::null;
	};

} // namespace Editor
} // namespace Pekan