#pragma once

#include "PekanGUIWindow.h"

#include "TextWidget.h"
#include "ColorEdit3Widget.h"
#include "DragFloat2Widget.h"

namespace Demo
{

	class Demo00_GUIWindow : public Pekan::PekanGUIWindow
	{
	public:

		Demo00_GUIWindow(Pekan::PekanApplication* application) : PekanGUIWindow(application) {}

		// Returns current value of the "clear color" parameter
		inline glm::vec4 getClearColor() const { return glm::vec4(gui.colorEditWidget.getValue(), 1.0f); }

		// Returns current value of the "position" parameter.
		inline glm::vec2 getPosition() const { return gui.positionWidget.getValue(); }

	private: /* functions */

		bool init() override;

		void _render() override;

		Pekan::GUIWindowProperties getProperties() override;

	private: /* variables */

		struct Widgets
		{
			Pekan::GUI::TextWidget textWidget_backgroundColor;
			Pekan::GUI::ColorEdit3Widget colorEditWidget;
			Pekan::GUI::TextWidget textWidget_position;
			Pekan::GUI::DragFloat2Widget positionWidget;
		} gui;
	};

} // namespace Demo