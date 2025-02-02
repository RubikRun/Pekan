#pragma once

struct GLFWwindow;

namespace Pekan
{
	// Pekan Engine itself,
	// containing core engine data and functionality.
	class PekanEngine
	{
	public:
		// Initializes the engine.
		// Creates a window for rendering graphics and GUI,
		// and initializes all libraries.
		bool init();

		// Cleans up the engine.
		// Closes the window, frees resources, and cleans up all libraries.
		void cleanup();

		bool runExercise0000() const;

	private: /* functions */

		// Creates the window, configures it for rendering
		bool createWindow(int width, int height);
		// Cleans up the window
		void cleanupWindow();

		// Loads OpenGL function pointers
		bool loadOpenGL() const;

		// Initializes ImGui library
		bool initImGui() const;
		// Renders ImGui's draw data.
		// Must be called after drawing GUI with ImGui to actually render the GUI. (that's a funny sentence lol)
		void renderImGui() const;
		// Cleans up ImGui library
		void cleanupImGui() const;

	private: /* variables */

		// Window for rendering graphics
		GLFWwindow* window = nullptr;
	};

} // namespace Pekan