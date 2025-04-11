#pragma once

#include <string>

namespace Pekan
{

	class PekanApplication;

	class KeyPressedEvent; class KeyReleasedEvent;
	class MouseMovedEvent; class MouseScrolledEvent; class MouseButtonPressedEvent; class MouseButtonReleasedEvent;
	class WindowResizedEvent; class WindowClosedEvent;

	class Layer
	{
		friend class PekanApplication;

	public:
		Layer(const std::string& name) : m_name(name) {}
		virtual ~Layer() = default;

		virtual bool init() { return true; }
		virtual void exit() {}
		virtual void update() {}
		virtual void render() {}

		inline const std::string& getName() const { return m_name; }

	private: /* functions */

		// Functions that are automatically called when an event occurs.
		// Each of these functions can be overriden by a derived class to handle the specific type of event when it occurs.
		// If an event is successfully handled in one of these functions, then that function should return true
		// to indicate to Pekan that the event is handled and it shouldn't be propagated further back the layer stack.
		virtual bool onKeyPressed(KeyPressedEvent& event) { return false; }
		virtual bool onKeyReleased(KeyReleasedEvent& event) { return false; }
		virtual bool onMouseMoved(MouseMovedEvent& event) { return false; }
		virtual bool onMouseScrolled(MouseScrolledEvent& event) { return false; }
		virtual bool onMouseButtonPressed(MouseButtonPressedEvent& event) { return false; }
		virtual bool onMouseButtonReleased(MouseButtonReleasedEvent& event) { return false; }
		virtual bool onWindowResized(WindowResizedEvent& event) { return false; }
		virtual bool onWindowClosed(WindowClosedEvent& event) { return false; }

	private: /* variables*/

		std::string m_name;
	};

} // namespace Pekan