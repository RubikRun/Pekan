#pragma once

#include "PekanGUIWindow.h"
using Pekan::PekanGUIWindow;

#include "CheckboxWidget.h"
#include "ButtonWidget.h"
#include "TextWidget.h"
#include "SliderIntWidget.h"
#include "ColorEdit4Widget.h"

namespace Demo
{

	class Demo01_Scene;

	class Demo01_GUIWindow : public PekanGUIWindow
	{
	public:

		Demo01_GUIWindow(Pekan::PekanApplication* application) : PekanGUIWindow(application) {}

		inline bool getMoveThirdSquare() const { return gui.checkboxWidget_moveThirdSquare.isChecked(); }
		inline int getSquareX(size_t idx) const { return gui.squares[idx].sliderWidget_x.getValue(); }
		inline int getSquareY(size_t idx) const { return gui.squares[idx].sliderWidget_y.getValue(); }
		inline int getSquareSize(size_t idx) const { return gui.squares[idx].sliderWidget_size.getValue(); }
		inline int getSquareRotation(size_t idx) const { return gui.squares[idx].sliderWidget_rotation.getValue(); }
		inline glm::vec4 getSquareColor(size_t idx) const { return gui.squares[idx].colorWidget.getValue(); }

		inline int getNumberOfSquares() const { return gui.squares.size(); }

	private: /* functions */

		bool init() override;

		void _render() override;

		void update(double deltaTime) override;

		Pekan::GUIWindowProperties getProperties() override;

	private: /* variables */

		struct Widgets
		{
			struct SquareWidgets
			{
				Pekan::GUI::TextWidget textWidget_name;
				Pekan::GUI::SliderIntWidget sliderWidget_x;
				Pekan::GUI::SliderIntWidget sliderWidget_y;
				Pekan::GUI::SliderIntWidget sliderWidget_size;
				Pekan::GUI::SliderIntWidget sliderWidget_rotation;
				Pekan::GUI::ColorEdit4Widget colorWidget;
			};

			Pekan::GUI::CheckboxWidget checkboxWidget_moveThirdSquare;
			Pekan::GUI::ButtonWidget buttonWidget_addSquare;
			std::vector<SquareWidgets> squares;
		} gui;
	};

} // namespace Demo