#include "Demo01_Application.h"

#include "Demo01_Scene.h"
#include "Demo01_GUIWindow.h"

using Pekan::PekanScene;

namespace Demo
{

	bool Demo01_Application::_init()
	{
		// Create demo scene and demo GUI window
		Demo01_Scene* demoScene = new Demo01_Scene();
		Demo01_GUIWindow* demoGuiWindow = new Demo01_GUIWindow();
		// Attach demo scene to demo GUI window
		demoGuiWindow->attachScene(demoScene);

		// Set base scene pointer and base GUI window pointer
		// to point to demo scene and demo GUI window
		m_scene = std::unique_ptr<PekanScene>(demoScene);
		m_guiWindow = std::unique_ptr<PekanGUIWindow>(demoGuiWindow);

		return true;
	}

} // namespace Demo