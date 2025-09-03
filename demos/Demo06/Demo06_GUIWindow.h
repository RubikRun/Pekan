#pragma once

#include "GUIWindow.h"

#include "TextWidget.h"
#include "SliderIntWidget.h"
#include "CheckboxWidget.h"
#include "ComboBoxWidget.h"
#include "FPSDisplayWidget.h"

namespace Demo
{

	class Demo06_GUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

		Demo06_GUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		bool init() override;

		// Returns current number of shapes
		int getNumberOfShapes() const { return gui.numberOfShapesWidget->getValue(); }
		// Returns maximum number of shapes allowed in GUI
		int getMaxNumberOfShapes() const { return gui.numberOfShapesWidget->getMax(); }

		bool isEnabledRectangles() const { return gui.showRectanglesWidget->isChecked(); }
		bool isEnabledCircles() const { return gui.showCirclesWidget->isChecked(); }
		bool isEnabledCirclesStatic() const { return gui.showCirclesStaticWidget->isChecked(); }
		bool isEnabledTriangles() const { return gui.showTrianglesWidget->isChecked(); }
		bool isEnabledPolygons() const { return gui.showPolygonsWidget->isChecked(); }
		bool isEnabledLines() const { return gui.showLinesWidget->isChecked(); }

		// Returns index of selected post-processing shader
		int getPpsIndex() const { return gui.ppsComboBoxWidget->getIndex(); }

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
			Pekan::GUI::ComboBoxWidget_Ptr ppsComboBoxWidget =          std::make_shared<Pekan::GUI::ComboBoxWidget>();
			Pekan::GUI::TextWidget_Ptr ppsLabel =                       std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::FPSDisplayWidget_Ptr fpsDisplayWidget =         std::make_shared<Pekan::GUI::FPSDisplayWidget>();
		} gui;
	};

} // namespace Demo