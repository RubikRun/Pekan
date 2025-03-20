#include "Demo02_Application.h"

#include "Demo02_Scene.h"
#include "Demo02_GUIWindow.h"

using Pekan::PekanScene;

namespace Demo
{

	bool Demo02_Application::_init()
	{
		// Create demo scene and demo GUI window
		Demo02_Scene* demoScene = new Demo02_Scene();
		Demo02_GUIWindow* demoGuiWindow = new Demo02_GUIWindow();
		// Attach demo scene to demo GUI window
		demoGuiWindow->attachScene(demoScene);

		// Set base scene pointer and base GUI window pointer
		// to point to demo scene and demo GUI window
		m_scene = std::unique_ptr<PekanScene>(demoScene);
		m_guiWindow = std::unique_ptr<PekanGUIWindow>(demoGuiWindow);

		return true;
	}

} // namespace Demo