#pragma once

#include "Window.h"
#include "Events/KeyEvents_Enums.h"
#include "Events/MouseEvents_Enums.h"

#include <glm/glm.hpp>

namespace Pekan
{

	class PekanApplication;

	// Pekan Engine itself.
	// This is a singleton/static class responsible for initializing and exiting the engine,
	// managing the lifetime of a graphics window, and handling all external libraries.
	class PekanEngine
	{
		friend class Window;

	public:

		// Initializes the engine
		// To be called once, at the beginning, before using the engine.
		static bool init(PekanApplication* application);

		// Exits the engine
		// To be called once, at the end, after finished using the engine.
		static void exit();

		// Returns a reference to the window where the current application is running
		inline static Window& getWindow() { return s_window; }

		// Returns a pointer to the current application
		inline static PekanApplication* getApplication() { return s_application; }

		///////////////////
		// INPUT POLLING //
		///////////////////

		// Checks if a given key from the keyboard is currently pressed or released,
		// or repeating which means that it had been pressed and held down for a bit, like half a second.
		static bool isKeyPressed(KeyCode key);
		static bool isKeyReleased(KeyCode key);
		static bool isKeyRepeating(KeyCode key);
		// Returns current mouse position, in pixels, relative to window's top-left corner
		static glm::vec2 getMousePosition();
		// Checks if mouse button is currently pressed/released.
		static bool isMouseButtonPressed(MouseButton button);
		static bool isMouseButtonReleased(MouseButton button);
		// Returns window's current size (resolution)
		static glm::ivec2 getWindowSize();

	private: /* functions */

		// Initializes ImGui library, and creates ImGui context
		static bool initImGui();
		// Exists ImGui library, and destroys ImGui context
		static void exitImGui();

	private: /* variables */

		// Window for rendering graphics
		static Window s_window;

		// Application currently using Pekan
		static PekanApplication* s_application;

		// Flag indicating if engine is initialized
		static bool isInitialized;
	};

} // namespace Pekan

#if !defined(NDEBUG)
// Macro that can be used for debug-only code.
#define PK_DEBUG_CODE(...) __VA_ARGS__
#else
#define PK_DEBUG_CODE(...)
#endif