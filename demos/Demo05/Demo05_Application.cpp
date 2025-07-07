#include "Demo05_Application.h"

#include "Demo05_Scene.h"
#include "Demo05_GUIWindow.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::ApplicationProperties;
using Pekan::WindowProperties;
using Pekan::LayerStack;

namespace Demo
{

	bool Demo05_Application::_init(LayerStack& layerStack)
	{
		// Create demo scene and demo GUI window
		std::shared_ptr<Demo05_Scene> demoScene = std::make_shared<Demo05_Scene>(this);
		std::shared_ptr<Demo05_GUIWindow> demoGuiWindow = std::make_shared<Demo05_GUIWindow>(this);
		// Attach demo GUI window to demo scene
		demoScene->attachGUIWindow(demoGuiWindow);

		// Add scene and GUI window to application's layers
		layerStack.pushLayer(demoScene);
		layerStack.pushLayer(demoGuiWindow);

		return true;
	}

	ApplicationProperties Demo05_Application::getProperties() const
	{
		ApplicationProperties props;
		props.fps = 60.0;
		props.windowProperties.title = getName();
		props.windowProperties.width = 1000;
		props.windowProperties.height = 1000;
		props.windowProperties.numberOfSamples = 16;
		return props;
	}

} // namespace Demo