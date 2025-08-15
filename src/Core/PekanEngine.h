#pragma once

#include "Window.h"

#include <glm/glm.hpp>

namespace Pekan { namespace GUI { class GUISystem; } }

namespace Pekan
{

	class PekanApplication;

	// Pekan Engine itself.
	// This is a singleton/static class responsible for initializing and exiting the engine.
	// It is the central point of Pekan:
	// - Manages all subsystems making up Pekan
	// - Keeps track of current application
	// - Manages the window where application is running
	// - Sends events from window to application
	class PekanEngine
	{

		// PekanApplication is declared as a friend to PekanEngine
		// because it needs private access to the window (to close it, enable VSync, etc.)
		// and we don't want other code to have this kind of access.
		friend class PekanApplication;

		// GUI is declared as a friend to PekanEngine
		// because it needs private access to the window (to act on its underlying GLFW window)
		// and we don't want other code to have this kind of access.
		friend class Pekan::GUI::GUISystem;

	public:

		// Initializes the engine.
		// To be called once, at the beginning, before using the engine.
		static bool init(PekanApplication* application);

		// Exits the engine.
		// To be called once, at the end, after having finished using the engine.
		static void exit();

		// Returns (a const reference to) the window where current application is running
		inline static const Window& getWindow() { return s_window; }

		// Returns (a pointer to) current application
		inline static PekanApplication* getApplication() { return s_application; }

		///////////////////
		// INPUT POLLING //
		///////////////////

		// Checks if a given key from the keyboard is currently pressed or released,
		// or repeating which means that it had been pressed and held down for a bit (like half a second).
		static bool isKeyPressed(KeyCode key);
		static bool isKeyReleased(KeyCode key);
		static bool isKeyRepeating(KeyCode key);
		// Returns current mouse position, in pixels, relative to window's top-left corner
		static glm::vec2 getMousePosition();
		// Returns current mouse position, in NDC (Normalized Device Coordinates),
		// which means from -1 to 1 in both the X and Y axes,
		// and the Y axis is pointing upwards.
		static glm::vec2 getMousePositionNdc();
		// Checks if mouse button is currently pressed/released.
		static bool isMouseButtonPressed(MouseButton button);
		static bool isMouseButtonReleased(MouseButton button);

	private: /* variables */

		// Window where current application is running
		static Window s_window;

		// Application currently using Pekan
		static PekanApplication* s_application;

		// Flag indicating if engine is initialized
		static bool s_isInitialized;
	};

} // namespace Pekan

#if !defined(NDEBUG)
// Macro that can be used for debug-only code.
#define PK_DEBUG_CODE(...) __VA_ARGS__
#else
#define PK_DEBUG_CODE(...)
#endif
