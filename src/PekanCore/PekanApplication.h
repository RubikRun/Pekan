#pragma once

#include "PekanScene.h"
#include "PekanGUIWindow.h"

#include <memory>

struct GLFWwindow;

namespace Pekan
{

	// A base class for all Pekan applications
	class PekanApplication
	{
	public:

		virtual ~PekanApplication() { exit(); }

		bool init();

		void run();

		void exit();

	private: /* functions */

		// Initializes the application.
		// 
		// To be implemented by derived classes
		// to set up the scene and guiWindow pointers.
		virtual bool _init() = 0;

	protected: /* variables */

		std::unique_ptr<PekanScene> m_scene;

		std::unique_ptr<PekanGUIWindow> m_guiWindow;

		// Flag indicating whether application should be rendered at full screen.
		// To be set by derived classes inside of the _init() function.
		bool m_isFullScreen = false;
	};

} // namespace Pekan