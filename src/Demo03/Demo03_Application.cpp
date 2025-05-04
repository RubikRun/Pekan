#include "Demo03_Application.h"

#include "Demo03_Scene.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::WindowProperties;
using Pekan::LayerStack;

namespace Demo
{

	bool Demo03_Application::_init(LayerStack& layerStack)
	{
		WindowProperties windowProperties;
		windowProperties.fullScreen = true;
		windowProperties.hideCursor = true;
		PekanEngine::createWindow(windowProperties);

		// Create demo scene
		Demo03_Scene* demoScene = new Demo03_Scene(this);
		// Add scene to application's layers
		layerStack.pushLayer(demoScene);

		setFPS(60.0);

		return true;
	}

} // namespace Demo