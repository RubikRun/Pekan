#include "Demo01_Application.h"

#include "Demo01_Scene.h"
#include "Demo01_GUIWindow.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::WindowProperties;

namespace Demo
{

	bool Demo01_Application::_init()
	{
		WindowProperties windowProperties;
		windowProperties.title = getName();
		PekanEngine::createWindow(windowProperties);

		// Create demo scene and demo GUI window
		Demo01_Scene* demoScene = new Demo01_Scene(this);
		Demo01_GUIWindow* demoGuiWindow = new Demo01_GUIWindow(this);
		// Attach demo scene to demo GUI window
		demoGuiWindow->attachScene(demoScene);

		// Add scene and GUI window to application's layers
		m_layerStack.pushLayer(demoScene);
		m_layerStack.pushLayer(demoGuiWindow);

		return true;
	}

} // namespace Demo