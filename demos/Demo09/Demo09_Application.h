#pragma once

#include "PekanApplication.h"

namespace Demo
{

    class Demo09_Application : public Pekan::PekanApplication
    {
        bool _fillLayerStack(Pekan::LayerStack& layerStack) override;
        std::string getName() const override { return "Entity Component System"; }
        Pekan::ApplicationProperties getProperties() const override;
    };

} // namespace Demo
