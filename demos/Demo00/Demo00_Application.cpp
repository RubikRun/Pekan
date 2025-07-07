#include "Demo00_Application.h"

#include "Demo00_Scene.h"
#include "Demo00_GUIWindow.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::ApplicationProperties;
using Pekan::WindowProperties;
using Pekan::LayerStack;

namespace Demo
{

	bool Demo00_Application::_fillLayerStack(LayerStack& layerStack)
	{
		// Create demo scene and demo GUI window
		std::shared_ptr<Demo00_Scene> demoScene = std::make_shared<Demo00_Scene>(this);
		std::shared_ptr<Demo00_GUIWindow> demoGuiWindow = std::make_shared<Demo00_GUIWindow>(this);
		// Attach demo GUI window to demo scene
		demoScene->attachGUIWindow(demoGuiWindow);

		// Add scene and GUI window to application's layers
		layerStack.pushLayer(demoScene);
		layerStack.pushLayer(demoGuiWindow);

		return true;
	}

	ApplicationProperties Demo00_Application::getProperties() const
	{
		ApplicationProperties props;
		props.windowProperties.title = getName();
		return props;
	}

} // namespace Demo