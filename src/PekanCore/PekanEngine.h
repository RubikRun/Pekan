#pragma once

#include "Logger/PekanLogger.h"
#include "Window.h"

#include <string>
#include <glm/glm.hpp>

namespace Pekan
{

	class PekanApplication;

	// Returns a user-friendly string from given OpenGL error code
	std::string _getGLErrorMessage(unsigned error);

#define _CLEAR_GL_ERRORS while (glGetError() != GL_NO_ERROR);
#define _LOG_GL_ERRORS { unsigned _error; while ((_error = glGetError()) != GL_NO_ERROR) { PK_LOG_ERROR(_getGLErrorMessage(_error), "OpenGL"); } }
// An error-checking macro for wrapping OpenGL calls.
// What it does is it clears all OpenGL errors from the error queue, then does the OpenGL call,
// and then loops over all new errors in the error queue and logs them using PekanLogger.
#define GLCall(x) _CLEAR_GL_ERRORS; x; _LOG_GL_ERRORS;

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

		static Window& getWindow() { return s_window; }

		///////////////////
		// INPUT POLLING //
		///////////////////

		// Checks if a given key from the keyboard is currently pressed or released,
		// or repeating which means that it had been pressed and held down for a bit, like half a second.
		static bool isKeyPressed(int key);
		static bool isKeyReleased(int key);
		static bool isKeyRepeating(int key);
		// Returns current mouse position, in pixels, relative to window's top-left corner
		static glm::vec2 getMousePosition();
		// Checks if mouse button is currently pressed/released.
		// If leftOrRight is true then left button is checked, otherwise right button is checked.
		static bool isMouseButtonPressed(bool leftOrRight);
		static bool isMouseButtonReleased(bool leftOrRight);
		// Returns window's current resolution
		static glm::ivec2 getWindowResolution();

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
