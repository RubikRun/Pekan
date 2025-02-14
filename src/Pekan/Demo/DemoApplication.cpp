#include "DemoApplication.h"

#include "DemoScene.h"
#include "DemoGUIWindow.h"

namespace Pekan
{

	bool DemoApplication::_init()
	{
		// Create demo scene and demo GUI window
		DemoScene* demoScene = new DemoScene();
		DemoGUIWindow* demoGuiWindow = new DemoGUIWindow();
		// Attach demo GUI window to demo scene
		demoScene->attachGUIWindow(demoGuiWindow);

		// Set base scene pointer and base GUI window pointer
		// to point to demo scene and demo GUI window
		scene = std::unique_ptr<PekanScene>(demoScene);
		guiWindow = std::unique_ptr<PekanGUIWindow>(demoGuiWindow);

		return true;
	}

} // namespace Pekan