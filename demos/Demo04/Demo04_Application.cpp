#include "Demo04_Application.h"

#include "Demo04_Scene.h"
#include "Demo04_GUIWindow.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::ApplicationProperties;
using Pekan::WindowProperties;
using Pekan::LayerStack;

namespace Demo
{

	bool Demo04_Application::_fillLayerStack(LayerStack& layerStack)
	{
		// Create demo scene and demo GUI window
		std::shared_ptr<Demo04_Scene> demoScene = std::make_shared<Demo04_Scene>(this);
		std::shared_ptr<Demo04_GUIWindow> demoGuiWindow = std::make_shared<Demo04_GUIWindow>(this);
		// Attach demo GUI window to demo scene
		demoScene->attachGUIWindow(demoGuiWindow);

		// Add scene and GUI window to application's layers
		layerStack.pushLayer(demoScene);
		layerStack.pushLayer(demoGuiWindow);

		return true;
	}

	ApplicationProperties Demo04_Application::getProperties() const
	{
		ApplicationProperties props;
		props.fps = 60.0;
		props.windowProperties.title = getName();
		props.windowProperties.width = 960;
		props.windowProperties.height = 960;
		props.numberOfSamples = 16;
		return props;
	}

} // namespace Demo