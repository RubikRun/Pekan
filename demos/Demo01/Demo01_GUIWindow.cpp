#include "Demo01_GUIWindow.h"
#include "PekanLogger.h"
#include "Demo01_Scene.h"
#include "PekanEngine.h"

using namespace Pekan;
using namespace Pekan::GUI;

namespace Demo
{

    bool Demo01_GUIWindow::init()
    {
        gui.checkboxWidget_moveThirdSquare = CheckboxWidget("Move third square slightly", false);
        gui.buttonWidget_addSquare = ButtonWidget("+");
        return true;
    }

	void Demo01_GUIWindow::_render()
	{
        gui.checkboxWidget_moveThirdSquare.render();
        gui.buttonWidget_addSquare.render();

        for (const Widgets::SquareWidgets& square : gui.squares)
        {
            square.textWidget_name.render();
            square.sliderWidget_x.render();
            square.sliderWidget_y.render();
            square.sliderWidget_size.render();
            square.sliderWidget_rotation.render();
            square.colorWidget.render();
        }
	}

    void Demo01_GUIWindow::update(double deltaTime)
    {
        static const glm::ivec2 windowSize = PekanEngine::getWindow().getSize();

        if (gui.buttonWidget_addSquare.isClicked())
        {
            // Create new square's widgets
            Widgets::SquareWidgets squareWidgets;
            const std::string squareName = std::string("Square ") + std::to_string(gui.squares.size() + 1);
            squareWidgets.textWidget_name = TextWidget(squareName.c_str());
            squareWidgets.sliderWidget_x = SliderIntWidget("X", windowSize.x / 2, 0, windowSize.x);
            squareWidgets.sliderWidget_y = SliderIntWidget("Y", windowSize.y / 2, 0, windowSize.y);
            squareWidgets.sliderWidget_size = SliderIntWidget("Size", 60, 0, std::max(windowSize.x, windowSize.y));
            squareWidgets.sliderWidget_rotation = SliderIntWidget("Rotation", 0, 0, 360);
            squareWidgets.colorWidget = ColorEdit4Widget("Color");
            // Add new square's widgets to the list of square widgets
            gui.squares.push_back(squareWidgets);
        }
    }

    Pekan::GUIWindowProperties Demo01_GUIWindow::getProperties()
    {
        GUIWindowProperties props;
        props.size = { 320, PekanEngine::getWindow().getSize().y };
        props.name = "Squares";
        return props;
    }

} // namespace Demo