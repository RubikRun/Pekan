#pragma once

#include <string>
#include <glm/glm.hpp>

struct GLFWwindow;

namespace Pekan
{
	class PekanEngine;

	// A window's properties grouped together in a convenient struct with default values
	struct WindowProperties
	{
		int width = 1280;
		int height = 720;
		std::string title = "Pekan Engine v0.1";
		bool fullScreen = false;
		bool hideCursor = false;
	};

	// A class representing an OS-independent window
	// where graphics can be rendered
	class Window
	{
		friend class PekanEngine;

	public:

		bool create(const WindowProperties& properties);
		void destroy();

		// Enables/Disables VSync - using FPS equal to monitor's refresh rate
		void enableVSync();
		void disableVSync();

		// Checks if window should be closed.
		// Returns true if the X button on the top-right has been pressed,
		// or the "should be closed" state is enabled    (see function setShouldBeClosed() below)
		bool shouldBeClosed() const;
		// Enables/disables the "should be closed" state, indicating that the window should be closed
		// on the next iteration of the main loop, or whenever possible.
		void setShouldBeClosed(bool enabled);

		// Swaps the front and back buffers of the window.
		// Shows on screen, on the front buffer, whatever has been rendered on the back buffer.
		// If VSync is enabled, this function will wait the correct amount of time before swapping the buffers and showing the new frame.
		void swapBuffers();

		// Checks if window is currently minimed (iconified)
		bool isMinimized() const;

		///////////////////
		// INPUT POLLING //
		///////////////////

		// Checks if a given key from the keyboard is currently pressed or released,
		// or repeating which means that it had been pressed and held down for a bit, like half a second.
		bool isKeyPressed(int key) const;
		bool isKeyReleased(int key) const;
		bool isKeyRepeating(int key) const;
		// Returns current mouse position, in pixels, relative to window's top-left corner
		glm::vec2 getMousePosition() const;
		// Checks if mouse button is currently pressed/released.
		// If leftOrRight is true then left button is checked, otherwise right button is checked.
		bool isMouseButtonPressed(bool leftOrRight) const;
		bool isMouseButtonReleased(bool leftOrRight) const;
		// Returns window's current resolution
		glm::ivec2 getResolution() const;
		// Returns frame buffer's current size
		glm::ivec2 getFrameBufferSize() const;

	private: /* functions */

		// Connects event callbacks with the window, so that they are actually called when an event occurs.
		void setEventCallbacks();

		// Event callbacks.
		// Functions that are called when an event occurs.
		// Each of these functions handles a specific type of event
		// by just sending it to Pekan's current application.
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouseMovedCallback(GLFWwindow* window, double xPos, double yPos);
		static void mouseScrolledCallback(GLFWwindow* window, double xOffset, double yOffset);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void windowResizedCallback(GLFWwindow* window, int width, int height);
		static void windowClosedCallback(GLFWwindow* window);

	private: /* variables */

		// Underlying GLFW window
		GLFWwindow* m_glfwWindow = nullptr;

		WindowProperties m_properties;
	};

} // namespace Pekan