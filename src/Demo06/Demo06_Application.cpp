#include "Demo06_Application.h"

#include "Demo06_Scene.h"
#include "Demo06_GUIWindow.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::WindowProperties;
using Pekan::LayerStack;

namespace Demo
{

	bool Demo06_Application::_init(LayerStack& layerStack)
	{
		WindowProperties windowProperties;
		windowProperties.title = getName();
		windowProperties.width = 1720;
		windowProperties.height = 980;
		PekanEngine::createWindow(windowProperties);

		// Create demo scene and demo GUI window
		Demo06_Scene* demoScene = new Demo06_Scene(this);
		Demo06_GUIWindow* demoGuiWindow = new Demo06_GUIWindow(this);
		// Attach demo GUI window to demo scene
		demoScene->attachGUIWindow(demoGuiWindow);

		// Add scene and GUI window to application's layers
		layerStack.pushLayer(demoScene);
		layerStack.pushLayer(demoGuiWindow);

		return true;
	}

} // namespace Demo