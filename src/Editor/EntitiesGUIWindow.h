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

	class EntitiesGUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

		EntitiesGUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		std::string getLayerName() const override { return "entities_gui_window"; }

		bool init() override;

	private: /* functions */

		Pekan::GUI::GUIWindowProperties getProperties() const override;

	private: /* variables */

		struct Widgets
		{
		} gui;
	};

} // namespace Editor
} // namespace Pekan