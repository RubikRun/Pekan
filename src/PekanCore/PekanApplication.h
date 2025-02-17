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

		std::unique_ptr<PekanScene> scene;

		std::unique_ptr<PekanGUIWindow> guiWindow;

		// A graphics window where application is rendered
		GLFWwindow* window = nullptr;

		// Width of application, in pixels
		int width = -1;
		// Height of application, in pixels
		int height = -1;
	};

} // namespace Pekan