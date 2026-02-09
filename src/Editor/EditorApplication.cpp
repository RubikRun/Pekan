#include "EditorApplication.h"

#include "EditorScene.h"
#include "EntitiesGUIWindow.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::ApplicationProperties;
using Pekan::WindowProperties;
using Pekan::LayerStack;

namespace Pekan
{
namespace Editor
{

	bool EditorApplication::_fillLayerStack(LayerStack& layerStack)
	{
		// Create scene and GUI window
		std::shared_ptr<EditorScene> scene = std::make_shared<EditorScene>(this);
		std::shared_ptr<EntitiesGUIWindow> entitiesGuiWindow = std::make_shared<EntitiesGUIWindow>(this);

		// Add scene and GUI window to application's layers
		layerStack.pushLayer(scene);
		layerStack.pushLayer(entitiesGuiWindow);

		return true;
	}

	ApplicationProperties EditorApplication::getProperties() const
	{
		ApplicationProperties props;
		props.windowProperties.title = getName();
		props.numberOfSamples = 16;
		props.fps = 60.0;
		return props;
	}

} // namespace Editor
} // namespace Pekan
