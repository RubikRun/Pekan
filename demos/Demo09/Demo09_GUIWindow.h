#pragma once

#include "GUIWindow.h"

#include "TextWidget.h"
#include "ColorEdit3Widget.h"
#include "DragFloat2Widget.h"

namespace Demo
{

    class Demo09_GUIWindow : public Pekan::GUI::GUIWindow
    {
    public:

        Demo09_GUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

        bool init() override;

    private: /* functions */

        Pekan::GUI::GUIWindowProperties getProperties() const override;

        void _render() const override;
    };

} // namespace Demo

