#pragma once

#include "Events/KeyEvents_Enums.h"
#include "Events/MouseEvents_Enums.h"

#include <string>
#include <glm/glm.hpp>

struct GLFWwindow;

namespace Pekan
{
	class PekanEngine;
	class ApplicationProperties;

	// A window's properties grouped together in a struct with default values
	struct WindowProperties
	{
		// Window's resolution, in pixels
		int width = 1280;
		int height = 720;

		// Initial position where the window will be spawned when the application is launched.
		// This is the position of the top-left corner of the window, in pixel coordinates.
		//
		// NOTE: A window's top bar is NOT considered part of the window,
		//       so it will be ABOVE this position.
		//       In other words, this position will be the top-left corner of the window,
		//       but the bottom-left corner of the bar.
		glm::ivec2 initialPosition = glm::ivec2(30, 40);

		// Window's title appearing on the top bar
		std::string title = "Pekan Engine v0.2";

		// Flag indicating if window should be full-screen,
		// matching screen's resolution and having no top bar.
		bool fullScreen = false;

		// Flag indicating if mouse's cursor should be hidden
		bool hideCursor = false;
	};

	// A class representing an OS-independent window
	// where graphics can be rendered
	class Window
	{

	public:

		// Creates a window with given application properties
		bool create(const ApplicationProperties& applicationProperties);
		// Destroys a window
		void destroy();

		// Enables/Disables VSync.
		// VSync means to target FPS equal to monitor's refresh rate
		void enableVSync();
		void disableVSync();

		// Checks if window should be closed.
		// Returns true if the X button on the top-right has been pressed,
		// or the "should be closed" state is enabled    (see function setShouldBeClosed() below)
		bool shouldBeClosed() const;
		// Enables/disables window's "should be closed" state, indicating that the window should be closed
		// on the next iteration of the main loop, or whenever possible.
		void setShouldBeClosed(bool enabled);

		// Swaps the front and back buffers of the window.
		// Shows on screen (on the front buffer) whatever has been rendered on the back buffer.
		// If VSync is enabled, this function will wait the correct amount of time before swapping the buffers and showing the new frame.
		void swapBuffers();

		// Checks if window is currently minimized (iconified)
		bool isMinimized() const;

		// Returns (a pointer to) the underlying GLFW window
		inline GLFWwindow* getGlfwWindow() { return m_glfwWindow; }

		///////////////////
		// INPUT POLLING //
		///////////////////

		// Checks if a given key from the keyboard is currently pressed or released,
		// or repeating which means that it had been pressed and held down for a bit, like half a second.
		bool isKeyPressed(KeyCode key) const;
		bool isKeyReleased(KeyCode key) const;
		bool isKeyRepeating(KeyCode key) const;
		// Returns current mouse position, in pixels, relative to window's top-left corner
		glm::vec2 getMousePosition() const;
		// Checks if mouse button is currently pressed/released.
		bool isMouseButtonPressed(MouseButton button) const;
		bool isMouseButtonReleased(MouseButton button) const;
		// Returns window's current size (resolution)
		glm::ivec2 getSize() const;
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

		// Window's properties
		WindowProperties m_properties;
	};

} // namespace Pekan