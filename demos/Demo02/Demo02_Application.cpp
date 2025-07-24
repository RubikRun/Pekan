#include "Demo02_Application.h"

#include "Demo02_Scene.h"
#include "Demo02_GUIWindow.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::ApplicationProperties;
using Pekan::WindowProperties;
using Pekan::LayerStack;

namespace Demo
{

	bool Demo02_Application::_fillLayerStack(LayerStack& layerStack)
	{
		// Create demo scene and demo GUI window
		std::shared_ptr<Demo02_Scene> demoScene = std::make_shared<Demo02_Scene>(this);
		std::shared_ptr<Demo02_GUIWindow> demoGuiWindow = std::make_shared<Demo02_GUIWindow>(this);
		// Attach demo GUI window to demo scene
		demoScene->attachGUIWindow(demoGuiWindow);

		// Add scene and GUI window to application's layers
		layerStack.pushLayer(demoScene);
		layerStack.pushLayer(demoGuiWindow);

		return true;
	}

	ApplicationProperties Demo02_Application::getProperties() const
	{
		ApplicationProperties props;
		props.windowProperties.title = getName();
		props.windowProperties.numberOfSamples = 16;
		return props;
	}

} // namespace Demo