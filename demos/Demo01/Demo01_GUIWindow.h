#pragma once

#include "GUIWindow.h"

#include "CheckboxWidget.h"
#include "ButtonWidget.h"
#include "TextWidget.h"
#include "SliderIntWidget.h"
#include "ColorEdit4Widget.h"

namespace Demo
{

	class Demo01_Scene;

	class Demo01_GUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

		Demo01_GUIWindow(Pekan::PekanApplication* application) : Pekan::GUI::GUIWindow(application) {}

		bool init() override;

		bool getMoveThirdSquare() const;
		int getSquareX(size_t idx) const;
		int getSquareY(size_t idx) const;
		int getSquareSize(size_t idx) const;
		int getSquareRotation(size_t idx) const;
		glm::vec4 getSquareColor(size_t idx) const;

		int getNumberOfSquares() const { return gui.squares.size(); }

	private: /* functions */

		void update(double deltaTime) override;

		Pekan::GUI::GUIWindowProperties getProperties() const override;

	private: /* variables */

		struct Widgets
		{
			struct SquareWidgets
			{
				Pekan::GUI::TextWidget_Ptr textWidget_name =               std::make_shared<Pekan::GUI::TextWidget>();
				Pekan::GUI::SliderIntWidget_Ptr sliderWidget_x =           std::make_shared<Pekan::GUI::SliderIntWidget>();
				Pekan::GUI::SliderIntWidget_Ptr sliderWidget_y =           std::make_shared<Pekan::GUI::SliderIntWidget>();
				Pekan::GUI::SliderIntWidget_Ptr sliderWidget_size =        std::make_shared<Pekan::GUI::SliderIntWidget>();
				Pekan::GUI::SliderIntWidget_Ptr sliderWidget_rotation =    std::make_shared<Pekan::GUI::SliderIntWidget>();
				Pekan::GUI::ColorEdit4Widget_Ptr colorWidget =             std::make_shared<Pekan::GUI::ColorEdit4Widget>();
			};

			Pekan::GUI::CheckboxWidget_Ptr checkboxWidget_moveThirdSquare =    std::make_shared<Pekan::GUI::CheckboxWidget>();
			Pekan::GUI::ButtonWidget_Ptr buttonWidget_addSquare =              std::make_shared<Pekan::GUI::ButtonWidget>();

			std::vector<SquareWidgets> squares;
		} gui;
	};

} // namespace Demo