#pragma once

#include "GUIWindow.h"

#include "ColorEdit3Widget.h"
#include "SliderFloatWidget.h"

namespace Demo
{

	class Demo05_GUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

		Demo05_GUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		bool init() override;

		// Returns current value of background color parameter
		glm::vec3 getBackgroundColor() const { return gui.backgroundColorWidget->getValue(); }

		// Returns current value of line thickness parameter
		float getLineThickness() const { return gui.lineThicknessWidget->getValue(); }

	private: /* functions */

		Pekan::GUI::GUIWindowProperties getProperties() const override;

	private: /* variables */

		struct Widgets
		{
			Pekan::GUI::ColorEdit3Widget_Ptr backgroundColorWidget =    std::make_shared<Pekan::GUI::ColorEdit3Widget>();
			Pekan::GUI::SliderFloatWidget_Ptr lineThicknessWidget =     std::make_shared<Pekan::GUI::SliderFloatWidget>();
		} gui;
	};

} // namespace Demo