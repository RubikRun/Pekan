#include "Demo00_Application.h"

#include "Demo00_Scene.h"
#include "Demo00_GUIWindow.h"

namespace Demo
{

	bool Demo00_Application::_init()
	{
		// Create demo scene and demo GUI window
		Demo00_Scene* demoScene = new Demo00_Scene();
		Demo00_GUIWindow* demoGuiWindow = new Demo00_GUIWindow();
		// Attach demo GUI window to demo scene
		demoScene->attachGUIWindow(demoGuiWindow);

		// Add scene and GUI window to application's layers
		m_layerStack.pushLayer(demoScene);
		m_layerStack.pushLayer(demoGuiWindow);

		return true;
	}

} // namespace Demo