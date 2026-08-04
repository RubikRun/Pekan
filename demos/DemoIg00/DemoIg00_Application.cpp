#include "DemoIg00_Application.h"

#include "DemoIg00_Scene.h"

#include "PekanEngine.h"
using Pekan::ApplicationProperties;
using Pekan::LayerStack;

namespace Demo
{

	bool DemoIg00_Application::_fillLayerStack(LayerStack& layerStack)
	{
		std::shared_ptr<DemoIg00_Scene> demoScene = std::make_shared<DemoIg00_Scene>(this);
		layerStack.pushLayer(demoScene);
		return true;
	}

	ApplicationProperties DemoIg00_Application::getProperties() const
	{
		ApplicationProperties props;
		props.fps = 60.0;
		props.numberOfSamples = 16;
		props.windowProperties.title = getName();
		props.windowProperties.width = 518;
		props.windowProperties.height = 922;
		return props;
	}

} // namespace Demo
