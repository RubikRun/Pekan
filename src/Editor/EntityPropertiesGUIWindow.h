#pragma once

#include "GUIWindow.h"

#include "TextWidget.h"
#include "ColorEdit3Widget.h"
#include "DragFloat2Widget.h"
#include "ComboBoxWidget.h"
#include "SeparatorWidget.h"
#include "SliderFloat2Widget.h"
#include "SliderFloatWidget.h"

namespace Pekan
{
namespace Editor
{

	class EntityPropertiesGUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

		EntityPropertiesGUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		std::string getLayerName() const override { return "entity_properties_gui_window"; }

	private: /* functions */

		bool _init() override;

		Pekan::GUI::GUIWindowProperties getProperties() const override;

	private: /* variables */

		struct Widgets
		{
		} gui;
	};

} // namespace Editor
} // namespace Pekan