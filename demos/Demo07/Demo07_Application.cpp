#include "Demo07_Application.h"

#include "Demo07_Scene.h"
#include "Demo07_GUIWindow.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::ApplicationProperties;
using Pekan::WindowProperties;
using Pekan::LayerStack;

namespace Demo
{

	bool Demo07_Application::_init(LayerStack& layerStack)
	{
		// Create demo scene and demo GUI window
		Demo07_Scene* demoScene = new Demo07_Scene(this);
		Demo07_GUIWindow* demoGuiWindow = new Demo07_GUIWindow(this);
		// Attach demo GUI window to demo scene
		demoScene->attachGUIWindow(demoGuiWindow);

		// Add scene and GUI window to application's layers
		layerStack.pushLayer(demoScene);
		layerStack.pushLayer(demoGuiWindow);

		return true;
	}

	ApplicationProperties Demo07_Application::getProperties() const
	{
		ApplicationProperties props;
		props.windowProperties.title = getName();
		props.windowProperties.numberOfSamples = 16;
		return props;
	}

} // namespace Demo