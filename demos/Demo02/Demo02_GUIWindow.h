#pragma once

#include "GUIWindow.h"

#include "ColorEdit3Widget.h"
#include "SeparatorWidget.h"
#include "SliderFloatWidget.h"
#include "CheckboxWidget.h"

namespace Demo
{
	class Demo02_Scene;

	class Demo02_GUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

		Demo02_GUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		bool init() override;

		glm::vec3 getFaceColor(size_t idx) const { return gui.colorEditWidgets[idx]->getValue(); }
		float getRotation() const { return gui.rotationWidget->getValue(); }
		float getFOV() const { return gui.fovWidget->getValue(); }
		float getCameraDist() const { return gui.cameraDistWidget->getValue(); }
		bool shouldHideFourthFace() const { return gui.hideFourthFaceWidget->isChecked(); }

	private: /* functions */

		Pekan::GUI::GUIWindowProperties getProperties() const override;

	private: /* variables */

		struct Widgets
		{
			Pekan::GUI::ColorEdit3Widget_Ptr colorEditWidgets[6] =
			{
				std::make_shared<Pekan::GUI::ColorEdit3Widget>(),
				std::make_shared<Pekan::GUI::ColorEdit3Widget>(),
				std::make_shared<Pekan::GUI::ColorEdit3Widget>(),
				std::make_shared<Pekan::GUI::ColorEdit3Widget>(),
				std::make_shared<Pekan::GUI::ColorEdit3Widget>(),
				std::make_shared<Pekan::GUI::ColorEdit3Widget>()
			};
			Pekan::GUI::SeparatorWidget_Ptr separatorWidget =        std::make_shared<Pekan::GUI::SeparatorWidget>();
			Pekan::GUI::SliderFloatWidget_Ptr rotationWidget =       std::make_shared<Pekan::GUI::SliderFloatWidget>();
			Pekan::GUI::SliderFloatWidget_Ptr fovWidget =            std::make_shared<Pekan::GUI::SliderFloatWidget>();
			Pekan::GUI::SliderFloatWidget_Ptr cameraDistWidget =     std::make_shared<Pekan::GUI::SliderFloatWidget>();
			Pekan::GUI::CheckboxWidget_Ptr hideFourthFaceWidget =    std::make_shared<Pekan::GUI::CheckboxWidget>();
		} gui;
	};

} // namespace Demo