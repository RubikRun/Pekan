#pragma once

#include "GUIWindow.h"

#include "TextWidget.h"
#include "SliderIntWidget.h"
#include "CheckboxWidget.h"
#include "SliderFloatWidget.h"
#include "FPSDisplayWidget.h"

namespace Demo
{

	class Demo08_GUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

		Demo08_GUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		bool init() override;

		// Returns current number of sprites
		int getNumberOfSprites() const { return gui.numberOfSpritesWidget->getValue(); }
		// Returns maximum number of sprites allowed in GUI
		int getMaxNumberOfSprites() const { return gui.numberOfSpritesWidget->getMax(); }
		// Returns mouse strength parameter from GUI
		float getMouseStrength() const { return gui.mouseStrengthWidget->getValue(); }

	private: /* functions */

		Pekan::GUI::GUIWindowProperties getProperties() const override;

	private: /* variables */

		struct Widgets
		{
			Pekan::GUI::TextWidget_Ptr numberOfSpritesLabel =            std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::SliderIntWidget_Ptr numberOfSpritesWidget =      std::make_shared<Pekan::GUI::SliderIntWidget>();
			Pekan::GUI::SliderFloatWidget_Ptr mouseStrengthWidget =      std::make_shared<Pekan::GUI::SliderFloatWidget>();
			Pekan::GUI::FPSDisplayWidget_Ptr fpsDisplayWidget =          std::make_shared<Pekan::GUI::FPSDisplayWidget>();
		} gui;
	};

} // namespace Demo