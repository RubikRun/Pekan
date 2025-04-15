#include "Demo02_Application.h"

#include "Demo02_Scene.h"
#include "Demo02_GUIWindow.h"

namespace Demo
{

	bool Demo02_Application::_init()
	{
		// Create demo scene and demo GUI window
		Demo02_Scene* demoScene = new Demo02_Scene();
		Demo02_GUIWindow* demoGuiWindow = new Demo02_GUIWindow();
		// Attach demo scene to demo GUI window
		demoGuiWindow->attachScene(demoScene);

		// Add scene and GUI window to application's layers
		m_layerStack.pushLayer(demoScene);
		m_layerStack.pushLayer(demoGuiWindow);

		setFPS(24.0);

		return true;
	}

} // namespace Demo