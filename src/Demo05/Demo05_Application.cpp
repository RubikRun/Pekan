#include "Demo05_Application.h"

#include "Demo05_Scene.h"
#include "Demo05_GUIWindow.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::WindowProperties;
using Pekan::LayerStack;

namespace Demo
{

	bool Demo05_Application::_init(LayerStack& layerStack)
	{
		WindowProperties windowProperties;
		windowProperties.title = getName();
		windowProperties.numberOfSamples = 16;
		PekanEngine::createWindow(windowProperties);

		// Create demo scene and demo GUI window
		Demo05_Scene* demoScene = new Demo05_Scene(this);
		Demo05_GUIWindow* demoGuiWindow = new Demo05_GUIWindow(this);
		// Attach demo GUI window to demo scene
		demoScene->attachGUIWindow(demoGuiWindow);

		// Add scene and GUI window to application's layers
		layerStack.pushLayer(demoScene);
		layerStack.pushLayer(demoGuiWindow);

		return true;
	}

} // namespace Demo