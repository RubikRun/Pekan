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

	bool Demo04_Application::_init(LayerStack& layerStack)
	{
		setFPS(60.0);

		// Create demo scene and demo GUI window
		Demo04_Scene* demoScene = new Demo04_Scene(this);
		Demo04_GUIWindow* demoGuiWindow = new Demo04_GUIWindow(this);
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
		props.windowProperties.title = getName();
		props.windowProperties.width = 960;
		props.windowProperties.height = 960;
		props.windowProperties.numberOfSamples = 16;
		return props;
	}

} // namespace Demo