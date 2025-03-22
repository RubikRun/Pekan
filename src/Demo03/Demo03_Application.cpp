#include "Demo03_Application.h"

#include "Demo03_Scene.h"

using Pekan::PekanScene;

namespace Demo
{

	bool Demo03_Application::_init()
	{
		// Create demo scene
		Demo03_Scene* demoScene = new Demo03_Scene();
		// Set base scene pointer to point to demo scene
		m_scene = std::unique_ptr<PekanScene>(demoScene);

		// TODO: enable these after development is finished
		//m_isFullScreen = true;
		//m_shouldHideCursor = true;

		return true;
	}

} // namespace Demo