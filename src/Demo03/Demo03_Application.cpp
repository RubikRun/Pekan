#include "Demo03_Application.h"

#include "Demo03_Scene.h"

namespace Demo
{

	bool Demo03_Application::_init()
	{
		// Create demo scene
		Demo03_Scene* demoScene = new Demo03_Scene();
		// Add scene to application's layers
		m_layerStack.pushLayer(demoScene);

		// BORIS TEMP
		//m_isFullScreen = true;
		//m_shouldHideCursor = true;

		return true;
	}

} // namespace Demo