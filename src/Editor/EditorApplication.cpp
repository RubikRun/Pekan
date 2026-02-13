#include "EditorApplication.h"

#include "EditorScene.h"
#include "EntitiesGUIWindow.h"
#include "EntityPropertiesGUIWindow.h"

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
		// Create scene and GUI windows
		std::shared_ptr<EditorScene> scene = std::make_shared<EditorScene>(this);
		std::shared_ptr<EntitiesGUIWindow> entitiesGuiWindow = std::make_shared<EntitiesGUIWindow>(this);
		std::shared_ptr<EntityPropertiesGUIWindow> entityPropertiesGuiWindow = std::make_shared<EntityPropertiesGUIWindow>(this);

		// Connect GUI windows to scene
		entitiesGuiWindow->setScene(scene);
		entitiesGuiWindow->setEntityPropertiesGUIWindow(entityPropertiesGuiWindow);
		entityPropertiesGuiWindow->setScene(scene);

		// Add scene and GUI windows to application's layers
		layerStack.pushLayer(scene);
		layerStack.pushLayer(entitiesGuiWindow);
		layerStack.pushLayer(entityPropertiesGuiWindow);

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
