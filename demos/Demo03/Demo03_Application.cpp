#include "Demo03_Application.h"

#include "Demo03_Scene.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::ApplicationProperties;
using Pekan::WindowProperties;
using Pekan::LayerStack;

namespace Demo
{

	bool Demo03_Application::_fillLayerStack(LayerStack& layerStack)
	{
		// Create demo scene
		std::shared_ptr<Demo03_Scene> demoScene = std::make_shared<Demo03_Scene>(this);
		// Add scene to application's layers
		layerStack.pushLayer(demoScene);

		return true;
	}

	ApplicationProperties Demo03_Application::getProperties() const
	{
		ApplicationProperties props;
		props.fps = 60.0;
		props.windowProperties.fullScreen = true;
		props.windowProperties.hideCursor = true;
		return props;
	}

} // namespace Demo