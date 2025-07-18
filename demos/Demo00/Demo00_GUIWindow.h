#pragma once

#include "GUIWindow.h"

#include "TextWidget.h"
#include "ColorEdit3Widget.h"
#include "DragFloat2Widget.h"

namespace Demo
{

	class Demo00_GUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

		Demo00_GUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		// Returns current value of the "clear color" parameter
		glm::vec4 getClearColor() const;

		// Returns current value of the "position" parameter.
		glm::vec2 getPosition() const;

	private: /* functions */

		bool init() override;

		Pekan::GUI::GUIWindowProperties getProperties() override;

	private: /* variables */

		struct Widgets
		{
			Pekan::GUI::TextWidget_Ptr textWidget_backgroundColor    = std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::ColorEdit3Widget_Ptr colorEditWidget         = std::make_shared<Pekan::GUI::ColorEdit3Widget>();
			Pekan::GUI::TextWidget_Ptr textWidget_position           = std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::DragFloat2Widget_Ptr positionWidget          = std::make_shared<Pekan::GUI::DragFloat2Widget>();
		} gui;
	};

} // namespace Demo