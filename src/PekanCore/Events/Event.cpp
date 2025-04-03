#include "Event.h"

#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"

namespace Pekan
{

	std::vector<KeyPressedCallback> EventHandler::s_keyPressedCallbacks;
	std::vector<KeyReleasedCallback> EventHandler::s_keyReleasedCallbacks;
	std::vector<MouseMovedCallback> EventHandler::s_mouseMovedCallbacks;
	std::vector<MouseScrolledCallback> EventHandler::s_mouseScrolledCallbacks;
	std::vector<MouseButtonPressedCallback> EventHandler::s_mouseButtonPressedCallbacks;
	std::vector<MouseButtonReleasedCallback> EventHandler::s_mouseButtonReleasedCallbacks;
	std::vector<WindowResizedCallback> EventHandler::s_windowResizedCallbacks;
	std::vector<WindowClosedCallback> EventHandler::s_windowClosedCallbacks;

	void EventHandler::handleKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		switch (action)
		{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, false);
				handleEvent(event, s_keyPressedCallbacks);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				handleEvent(event, s_keyReleasedCallbacks);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, true);
				handleEvent(event, s_keyPressedCallbacks);
				break;
			}
		}
	}

	void EventHandler::handleMouseMovedEvent(GLFWwindow* window, double xPos, double yPos)
	{
		MouseMovedEvent event = { float(xPos), float(yPos) };
		handleEvent(event, s_mouseMovedCallbacks);
	}

	void EventHandler::handleMouseScrolledEvent(GLFWwindow* window, double xOffset, double yOffset)
	{
		MouseScrolledEvent event = { float(xOffset), float(yOffset) };
		handleEvent(event, s_mouseScrolledCallbacks);
	}

	void EventHandler::handleMouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
	{
		switch (action)
		{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				handleEvent(event, s_mouseButtonPressedCallbacks);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				handleEvent(event, s_mouseButtonReleasedCallbacks);
				break;
			}
		}
	}

	void EventHandler::handleWindowResizedEvent(GLFWwindow* window, int width, int height)
	{
		WindowResizedEvent event(width, height);
		handleEvent(event, s_windowResizedCallbacks);
	}

	void EventHandler::handleWindowClosedEvent(GLFWwindow* window)
	{
		WindowClosedEvent event;
		handleEvent(event, s_windowClosedCallbacks);
	}

} // namespace Pekan