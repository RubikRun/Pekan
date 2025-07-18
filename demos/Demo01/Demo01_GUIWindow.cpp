#include "Demo01_GUIWindow.h"
#include "PekanLogger.h"
#include "Demo01_Scene.h"
#include "PekanEngine.h"

using namespace Pekan;
using namespace Pekan::GUI;

namespace Demo
{

    bool Demo01_GUIWindow::getMoveThirdSquare() const
    {
        PK_ASSERT_QUICK(gui.checkboxWidget_moveThirdSquare != nullptr);
        return gui.checkboxWidget_moveThirdSquare->isChecked();
    }
    int Demo01_GUIWindow::getSquareX(size_t idx) const
    {
        PK_ASSERT_QUICK(idx >= 0 && idx < gui.squares.size() && gui.squares[idx].sliderWidget_x != nullptr);
        return gui.squares[idx].sliderWidget_x->getValue();
    }
    int Demo01_GUIWindow::getSquareY(size_t idx) const
    {
        PK_ASSERT_QUICK(idx >= 0 && idx < gui.squares.size() && gui.squares[idx].sliderWidget_y != nullptr);
        return gui.squares[idx].sliderWidget_y->getValue();
    }
    int Demo01_GUIWindow::getSquareSize(size_t idx) const
    {
        PK_ASSERT_QUICK(idx >= 0 && idx < gui.squares.size() && gui.squares[idx].sliderWidget_size != nullptr);
        return gui.squares[idx].sliderWidget_size->getValue();
    }
    int Demo01_GUIWindow::getSquareRotation(size_t idx) const
    {
        PK_ASSERT_QUICK(idx >= 0 && idx < gui.squares.size() && gui.squares[idx].sliderWidget_rotation != nullptr);
        return gui.squares[idx].sliderWidget_rotation->getValue();
    }
    glm::vec4 Demo01_GUIWindow::getSquareColor(size_t idx) const
    {
        PK_ASSERT_QUICK(idx >= 0 && idx < gui.squares.size() && gui.squares[idx].colorWidget != nullptr);
        return gui.squares[idx].colorWidget->getValue();
    }

    bool Demo01_GUIWindow::init()
    {
        gui.checkboxWidget_moveThirdSquare->create(this, "Move third square slightly", false);
        gui.buttonWidget_addSquare->create(this, "+");
        return true;
    }

    void Demo01_GUIWindow::update(double deltaTime)
    {
        static const glm::ivec2 windowSize = PekanEngine::getWindow().getSize();

        if (gui.buttonWidget_addSquare->isClicked())
        {
            // Create new square's widgets
            Widgets::SquareWidgets squareWidgets;
            const std::string squareName = std::string("Square ") + std::to_string(gui.squares.size() + 1);
            squareWidgets.textWidget_name->create(this, squareName.c_str());
            squareWidgets.sliderWidget_x->create(this, "X", windowSize.x / 2, 0, windowSize.x);
            squareWidgets.sliderWidget_y->create(this, "Y", windowSize.y / 2, 0, windowSize.y);
            squareWidgets.sliderWidget_size->create(this, "Size", 60, 0, std::max(windowSize.x, windowSize.y));
            squareWidgets.sliderWidget_rotation->create(this, "Rotation", 0, 0, 360);
            squareWidgets.colorWidget->create(this, "Color");
            // Add new square's widgets to the list of square widgets
            gui.squares.push_back(squareWidgets);
        }
    }

    GUIWindowProperties Demo01_GUIWindow::getProperties()
    {
        GUIWindowProperties props;
        props.size = { 320, PekanEngine::getWindow().getSize().y };
        props.name = "Squares";
        return props;
    }

} // namespace Demo