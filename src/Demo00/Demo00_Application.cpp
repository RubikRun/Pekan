#include "Demo00_Application.h"

#include "Demo00_Scene.h"
#include "Demo00_GUIWindow.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::WindowProperties;

namespace Demo
{

	bool Demo00_Application::_init()
	{
		WindowProperties windowProperties;
		windowProperties.title = getName();
		PekanEngine::createWindow(windowProperties);

		// Create demo scene and demo GUI window
		Demo00_Scene* demoScene = new Demo00_Scene(this);
		Demo00_GUIWindow* demoGuiWindow = new Demo00_GUIWindow(this);
		// Attach demo GUI window to demo scene
		demoScene->attachGUIWindow(demoGuiWindow);

		// Add scene and GUI window to application's layers
		m_layerStack.pushLayer(demoScene);
		m_layerStack.pushLayer(demoGuiWindow);

		return true;
	}

} // namespace Demo