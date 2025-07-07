#include "Demo01_Application.h"

#include "Demo01_Scene.h"
#include "Demo01_GUIWindow.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::ApplicationProperties;
using Pekan::WindowProperties;
using Pekan::LayerStack;

namespace Demo
{

	bool Demo01_Application::_fillLayerStack(LayerStack& layerStack)
	{
		// Create demo scene and demo GUI window
		std::shared_ptr<Demo01_Scene> demoScene = std::make_shared<Demo01_Scene>(this);
		std::shared_ptr<Demo01_GUIWindow> demoGuiWindow = std::make_shared<Demo01_GUIWindow>(this);
		// Attach demo scene to demo GUI window and attach demo GUI window to demo scene
		demoGuiWindow->attachScene(demoScene);
		demoScene->attachGUIWindow(demoGuiWindow);

		// Add scene and GUI window to application's layers
		layerStack.pushLayer(demoScene);
		layerStack.pushLayer(demoGuiWindow);

		return true;
	}

	ApplicationProperties Demo01_Application::getProperties() const
	{
		ApplicationProperties props;
		props.windowProperties.title = getName();
		props.windowProperties.numberOfSamples = 16;
		return props;
	}

} // namespace Demo