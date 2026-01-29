#include "Demo08_Application.h"

#include "Demo08_Scene.h"
#include "Demo08_GUIWindow.h"

#include "PekanEngine.h"
using Pekan::ApplicationProperties;
using Pekan::LayerStack;

namespace Demo
{

	bool Demo08_Application::_fillLayerStack(LayerStack& layerStack)
	{
		// Create demo scene and demo GUI window
		std::shared_ptr<Demo08_Scene> demoScene = std::make_shared<Demo08_Scene>(this);
		std::shared_ptr<Demo08_GUIWindow> demoGuiWindow = std::make_shared<Demo08_GUIWindow>(this);
		// Attach demo GUI window to demo scene
		demoScene->attachGUIWindow(demoGuiWindow);

		// Add scene and GUI window to application's layers
		layerStack.pushLayer(demoScene);
		layerStack.pushLayer(demoGuiWindow);

		// Set scene's parent to be GUI window
		// because we need some default GUI values to initialize the scene
		demoScene->setParent(demoGuiWindow);

		return true;
	}

	ApplicationProperties Demo08_Application::getProperties() const
	{
		ApplicationProperties props;
		props.fps = 60.0;
		props.windowProperties.title = getName();
		props.windowProperties.width = 1720;
		props.windowProperties.height = 980;
		props.numberOfSamples = 16;
		return props;
	}

} // namespace Demo