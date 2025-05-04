#include "Demo02_Application.h"

#include "Demo02_Scene.h"
#include "Demo02_GUIWindow.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::WindowProperties;

namespace Demo
{

	bool Demo02_Application::_init()
	{
		WindowProperties windowProperties;
		windowProperties.title = getName();
		PekanEngine::createWindow(windowProperties);

		// Create demo scene and demo GUI window
		Demo02_Scene* demoScene = new Demo02_Scene(this);
		Demo02_GUIWindow* demoGuiWindow = new Demo02_GUIWindow(this);
		// Attach demo scene to demo GUI window
		demoGuiWindow->attachScene(demoScene);

		// Add scene and GUI window to application's layers
		m_layerStack.pushLayer(demoScene);
		m_layerStack.pushLayer(demoGuiWindow);

		return true;
	}

} // namespace Demo