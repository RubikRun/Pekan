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
		bool init();

		// Exits the engine
		// - destroying graphics window
		// - destroying GUI window
		// - freeing all resources
		// 
		// To be called once, at the end, after finished using the engine.
		void exit();

		// TEMP
		GLFWwindow* getWindow() { return window; }

		// TEMP
		int getWindowWidth();
		int getWindowHeight();

	private: /* functions */

		// Creates the graphics window,
		// and configures it for rendering.
		bool createWindow(int width, int height);
		// Destroys the graphics window.
		void destroyWindow();

		// Loads OpenGL function pointers
		bool loadOpenGL() const;

		// Initializes ImGui library, and creates ImGui context
		bool initImGui() const;
		// Renders ImGui's draw data.
		// Must be called after drawing GUI with ImGui to actually render the GUI. (that's a funny sentence lol)
		void renderImGui() const;
		// Exists ImGui library, and destroys ImGui context
		void exitImGui() const;

	private: /* variables */

		// Window for rendering graphics
		GLFWwindow* window = nullptr;
	};

} // namespace Pekan