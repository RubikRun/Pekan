#include "Demo00_Application.h"

#include "Demo00_Scene.h"
#include "Demo00_GUIWindow.h"

using Pekan::PekanScene;

namespace Demo
{

	bool Demo00_Application::_init()
	{
		// Create demo scene and demo GUI window
		Demo00_Scene* demoScene = new Demo00_Scene();
		Demo00_GUIWindow* demoGuiWindow = new Demo00_GUIWindow();
		// Attach demo GUI window to demo scene
		demoScene->attachGUIWindow(demoGuiWindow);

		// Set base scene pointer and base GUI window pointer
		// to point to demo scene and demo GUI window
		scene = std::unique_ptr<PekanScene>(demoScene);
		guiWindow = std::unique_ptr<PekanGUIWindow>(demoGuiWindow);

		return true;
	}

} // namespace Demo