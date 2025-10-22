#include "Demo09_Application.h"

#include "Demo09_Scene.h"
#include "Demo09_GUIWindow.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::ApplicationProperties;
using Pekan::WindowProperties;
using Pekan::LayerStack;

namespace Demo
{

    bool Demo09_Application::_fillLayerStack(LayerStack& layerStack)
    {
        // Create demo scene and demo GUI window
        std::shared_ptr<Demo09_Scene> demoScene = std::make_shared<Demo09_Scene>(this);
        std::shared_ptr<Demo09_GUIWindow> demoGuiWindow = std::make_shared<Demo09_GUIWindow>(this);

        // Add scene and GUI window to application's layers
        layerStack.pushLayer(demoScene);
        layerStack.pushLayer(demoGuiWindow);

        return true;
    }

    ApplicationProperties Demo09_Application::getProperties() const
    {
        ApplicationProperties props;
        props.windowProperties.title = getName();
        return props;
    }

} // namespace Demo
