#include "Demo07_Application.h"

#include "Demo07_Scene.h"
#include "Demo07_GUIWindow.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::WindowProperties;
using Pekan::LayerStack;

namespace Demo
{

	bool Demo07_Application::_init(LayerStack& layerStack)
	{
		WindowProperties windowProperties;
		windowProperties.title = getName();
		windowProperties.numberOfSamples = 16;
		PekanEngine::createWindow(windowProperties);

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

} // namespace Demo