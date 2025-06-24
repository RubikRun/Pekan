#include "Demo04_Application.h"

#include "Demo04_Scene.h"
#include "Demo04_GUIWindow.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::WindowProperties;
using Pekan::LayerStack;

namespace Demo
{

	bool Demo04_Application::_init(LayerStack& layerStack)
	{
		WindowProperties windowProperties;
		windowProperties.title = getName();
		windowProperties.width = 960;
		windowProperties.height = 960;
		windowProperties.numberOfSamples = 16;
		PekanEngine::createWindow(windowProperties);

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

} // namespace Demo