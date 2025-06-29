#pragma once

#include "Logger/PekanLogger.h"
#include "Window.h"
#include "Events/KeyEvents_Enums.h"
#include "Events/MouseEvents_Enums.h"

#include <string>
#include <glm/glm.hpp>

#define _CLEAR_GL_ERRORS while (glGetError() != GL_NO_ERROR);
#define _LOG_GL_ERRORS { unsigned _error; while ((_error = glGetError()) != GL_NO_ERROR) { PK_LOG_ERROR(_getGLErrorMessage(_error), "OpenGL"); } }
// An error-checking macro for wrapping OpenGL calls.
// What it does is it clears all OpenGL errors from the error queue, then does the OpenGL call,
// and then loops over all new errors in the error queue and logs them using PekanLogger.
#define GLCall(x) _CLEAR_GL_ERRORS; x; _LOG_GL_ERRORS;

namespace Pekan
{

	class PekanApplication;

	// Returns a user-friendly string from given OpenGL error code
	std::string _getGLErrorMessage(unsigned error);

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

		// Creates a window with given properties, where graphics can be rendered.
		// Sets up OpenGL and ImGui for this window.
		//
		// NOTE: Currently Pekan supports only 1 window at a time,
		// so this function must be called at most once by an application.
		static bool createWindow(WindowProperties properties);

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

		// Loads OpenGL function pointers
		static bool loadOpenGL();

		// Initializes ImGui library, and creates ImGui context
		static bool initImGui();
		// Exists ImGui library, and destroys ImGui context
		static void exitImGui();

	private: /* variables */

		// Window for rendering graphics
		static Window s_window;

		// Application currently using Pekan
		static PekanApplication* s_application;

		static bool isInitialized;
		static bool isWindowCreated;
	};

} // namespace Pekan

#if !defined(NDEBUG)
// Macro that can be used for debug-only code.
#define PK_DEBUG_CODE(...) __VA_ARGS__
#else
#define PK_DEBUG_CODE(...)
#endif