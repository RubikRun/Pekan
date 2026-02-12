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

	class EntityPropertiesGUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

		EntityPropertiesGUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		std::string getLayerName() const override { return "entity_properties_gui_window"; }

		// Sets the entity whose properties will be displayed in this GUI window
		void setEntity(entt::entity entity);

	private: /* functions */

		bool _init() override;

		Pekan::GUI::GUIWindowProperties getProperties() const override;

	private: /* variables */

		struct Widgets
		{
			Pekan::GUI::TextWidget_Ptr entityInfoTextWidget = std::make_shared<Pekan::GUI::TextWidget>();
		} gui;

		// Entity whose properties are currently displayed in this GUI window.
		entt::entity m_entity = entt::null;
	};

} // namespace Editor
} // namespace Pekan