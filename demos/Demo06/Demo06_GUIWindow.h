#pragma once

#include "GUIWindow.h"

#include "TextWidget.h"
#include "SliderIntWidget.h"
#include "CheckboxWidget.h"
#include "FPSDisplayWidget.h"

namespace Demo
{

	class Demo06_GUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

		Demo06_GUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		bool init() override;

		void _exit() override;

		// Returns current number of shapes
		int getNumberOfShapes() const { return gui.numberOfShapesWidget->getValue(); }

		bool isEnabledRectangles() const { return gui.showRectanglesWidget->isChecked(); }
		bool isEnabledCircles() const { return gui.showCirclesWidget->isChecked(); }
		bool isEnabledCirclesStatic() const { return gui.showCirclesStaticWidget->isChecked(); }
		bool isEnabledTriangles() const { return gui.showTrianglesWidget->isChecked(); }
		bool isEnabledPolygons() const { return gui.showPolygonsWidget->isChecked(); }
		bool isEnabledLines() const { return gui.showLinesWidget->isChecked(); }

	private: /* functions */

		Pekan::GUI::GUIWindowProperties getProperties() const override;

	private: /* variables */

		struct Widgets
		{
			Pekan::GUI::TextWidget_Ptr numberOfShapesLabel =            std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::SliderIntWidget_Ptr numberOfShapesWidget =      std::make_shared<Pekan::GUI::SliderIntWidget>();
			Pekan::GUI::CheckboxWidget_Ptr showRectanglesWidget =       std::make_shared<Pekan::GUI::CheckboxWidget>();
			Pekan::GUI::CheckboxWidget_Ptr showCirclesWidget =          std::make_shared<Pekan::GUI::CheckboxWidget>();
			Pekan::GUI::CheckboxWidget_Ptr showCirclesStaticWidget =    std::make_shared<Pekan::GUI::CheckboxWidget>();
			Pekan::GUI::CheckboxWidget_Ptr showTrianglesWidget =        std::make_shared<Pekan::GUI::CheckboxWidget>();
			Pekan::GUI::CheckboxWidget_Ptr showPolygonsWidget =         std::make_shared<Pekan::GUI::CheckboxWidget>();
			Pekan::GUI::CheckboxWidget_Ptr showLinesWidget =            std::make_shared<Pekan::GUI::CheckboxWidget>();
			Pekan::GUI::FPSDisplayWidget_Ptr fpsDisplayWidget =         std::make_shared<Pekan::GUI::FPSDisplayWidget>();
		} gui;

		// We need to manually keep track of whether the GUI window is initialized,
		// because our scene depends on the GUI window,
		// meaning that the GUI window needs to be initialized first.
		// However, in the layer stack we have the GUI window AFTER the scene,
		// because it needs to be drawn on top,
		// so it will be automatically initialized AFTER the scene.
		// To make this work, the scene will manually initialize the GUI window
		// upon its own initialization, and then the GUI window will be initialized again
		// automatically by Pekan.
		// We don't want to initialize the GUI window twice so we'll just skip initialization
		// if it's already initialized.
		bool m_isInitialized = false;
	};

} // namespace Demo