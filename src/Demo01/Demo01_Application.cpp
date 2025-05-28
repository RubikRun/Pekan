#include "Demo01_Application.h"

#include "Demo01_Scene.h"
#include "Demo01_GUIWindow.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::WindowProperties;
using Pekan::LayerStack;

namespace Demo
{

	bool Demo01_Application::_init(LayerStack& layerStack)
	{
		WindowProperties windowProperties;
		windowProperties.title = getName();
		windowProperties.numberOfSamples = 16;
		PekanEngine::createWindow(windowProperties);

		// Create demo scene and demo GUI window
		Demo01_Scene* demoScene = new Demo01_Scene(this);
		Demo01_GUIWindow* demoGuiWindow = new Demo01_GUIWindow(this);
		// Attach demo scene to demo GUI window
		demoGuiWindow->attachScene(demoScene);

		// Add scene and GUI window to application's layers
		layerStack.pushLayer(demoScene);
		layerStack.pushLayer(demoGuiWindow);

		return true;
	}

} // namespace Demo