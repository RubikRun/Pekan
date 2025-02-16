#pragma once

struct GLFWwindow;

namespace Pekan
{

	// Pekan Engine itself
	class PekanEngine
	{
	public:

		// Initializes the engine
		// - creating a graphics window
		// - creating a GUI window
		// - initializing all libraries
		// 
		// To be called once, at the beginning, before using the engine.
		static bool init();

		// Exits the engine
		// - destroying graphics window
		// - destroying GUI window
		// - freeing all resources
		// 
		// To be called once, at the end, after finished using the engine.
		static void exit();

		// TEMP
		static GLFWwindow* getWindow() { return window; }

		// TEMP
		static int getWindowWidth();
		static int getWindowHeight();

	private: /* functions */

		// Creates the graphics window,
		// and configures it for rendering.
		static bool createWindow(int width, int height);
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
		static GLFWwindow* window;
	};

} // namespace Pekan
