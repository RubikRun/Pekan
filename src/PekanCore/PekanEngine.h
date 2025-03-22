#pragma once

#include "Logger/PekanLogger.h"

#include <string>

struct GLFWwindow;

#define PK_OPENGL_VERSION_MAJOR 4
#define PK_OPENGL_VERSION_MINOR 3
#define PK_GLSL_VERSION "#version 330 core"

namespace Pekan
{
	// Returns a user-friendly string from fiven OpenGL error code
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
	public:

		// Initializes the engine
		// - creating a graphics window
		// - creating a GUI window
		// - initializing all libraries
		// 
		// To be called once, at the beginning, before using the engine.
		static bool init(bool fullScreen = false, bool hideCursor = false);

		// Exits the engine
		// - destroying graphics window
		// - destroying GUI window
		// - freeing all resources
		// 
		// To be called once, at the end, after finished using the engine.
		static void exit();

		static GLFWwindow* getWindow() { return s_window; }

		static int getWindowWidth();
		static int getWindowHeight();

	// TODO: Remove the following functions after creating an event system
	public: /* TEMPORARY */
		static bool isKeyPressed_W();
		static bool isKeyPressed_A();
		static bool isKeyPressed_S();
		static bool isKeyPressed_D();


	private: /* functions */

		// Creates the graphics window,
		// and configures it for rendering.
		static bool createWindow(int width, int height, bool fullScreen, bool hideCursor);
		// Destroys the graphics window.
		static void destroyWindow();

		// Loads OpenGL function pointers
		static bool loadOpenGL();

		// Initializes ImGui library, and creates ImGui context
		static bool initImGui();
		// Renders ImGui's draw data.
		// Must be called after drawing GUI with ImGui to actually render the GUI. (that's a funny sentence lol)
		static void renderImGui();
		// Exists ImGui library, and destroys ImGui context
		static void exitImGui();

	private: /* variables */

		// Window for rendering graphics
		static GLFWwindow* s_window;
	};

} // namespace Pekan
