#include "Demo06_Application.h"

#include "Demo06_Scene.h"
#include "Demo06_GUIWindow.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::ApplicationProperties;
using Pekan::WindowProperties;
using Pekan::LayerStack;

namespace Demo
{

	bool Demo06_Application::_fillLayerStack(LayerStack& layerStack)
	{
		// Create demo scene and demo GUI window
		std::shared_ptr<Demo06_Scene> demoScene = std::make_shared<Demo06_Scene>(this);
		std::shared_ptr<Demo06_GUIWindow> demoGuiWindow = std::make_shared<Demo06_GUIWindow>(this);
		// Attach demo GUI window to demo scene
		demoScene->attachGUIWindow(demoGuiWindow);

		// Add scene and GUI window to application's layers
		layerStack.pushLayer(demoScene);
		layerStack.pushLayer(demoGuiWindow);

		return true;
	}

	ApplicationProperties Demo06_Application::getProperties() const
	{
		ApplicationProperties props;
		props.fps = 60.0;
		props.windowProperties.title = getName();
		props.windowProperties.width = 1720;
		props.windowProperties.height = 980;
		props.numberOfSamples = 16;
		return props;
	}

} // namespace Demo