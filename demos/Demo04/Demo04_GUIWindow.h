#pragma once

#include "GUIWindow.h"
#include "ColorEdit3Widget.h"
#include "DragFloat2Widget.h"
#include "ComboBoxWidget.h"
#include "CheckboxWidget.h"

namespace Demo
{

	class Demo04_GUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

		Demo04_GUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		bool init() override;

		// Returns current value of background color
		glm::vec4 getBackgroundColor() const { return glm::vec4(gui.backgroundColorWidget->getValue(), 1.0f); }

		// Returns current value of position
		glm::vec2 getPosition() const { return gui.positionWidget->getValue(); }

		// Checks if shapes are currently enabled
		bool isEnabledShapes() const { return gui.enableShapesWidget->isChecked(); }

		// Returns currently selected shader index
		int getShaderIdx() const { return gui.shaderWidget->getIndex(); }

		// Checks if triangle's vertices should be reversed (CW order instead of CCW)
		bool getReverseTriangleOrientation() const { return gui.reverseTriangleOrientationWidget->isChecked(); }

		// Checks if polygon's vertices should be reversed (CW order instead of CCW)
		bool getReversePolygonOrientation() const { return gui.reversePolygonOrientationWidget->isChecked(); }

		// Checks if face culling should be enabled
		bool getEnabledFaceCulling() const { return gui.enableFaceCullingWidget->isChecked(); }

	private: /* functions */

		Pekan::GUI::GUIWindowProperties getProperties() const override;

	private: /* variables */

		struct Widgets
		{
			Pekan::GUI::ColorEdit3Widget_Ptr backgroundColorWidget =             std::make_shared<Pekan::GUI::ColorEdit3Widget>();
			Pekan::GUI::DragFloat2Widget_Ptr positionWidget =                    std::make_shared<Pekan::GUI::DragFloat2Widget>();
			Pekan::GUI::ComboBoxWidget_Ptr shaderWidget =                        std::make_shared<Pekan::GUI::ComboBoxWidget>();
			Pekan::GUI::CheckboxWidget_Ptr enableShapesWidget =                  std::make_shared<Pekan::GUI::CheckboxWidget>();
			Pekan::GUI::CheckboxWidget_Ptr reverseTriangleOrientationWidget =    std::make_shared<Pekan::GUI::CheckboxWidget>();
			Pekan::GUI::CheckboxWidget_Ptr reversePolygonOrientationWidget =     std::make_shared<Pekan::GUI::CheckboxWidget>();
			Pekan::GUI::CheckboxWidget_Ptr enableFaceCullingWidget =             std::make_shared<Pekan::GUI::CheckboxWidget>();
		} gui;
	};

} // namespace Demo