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
		Layer(const std::string& name, PekanApplication* application) : m_name(name), m_application(application) {}
		virtual ~Layer() = default;

		virtual bool init() { return true; }
		virtual void exit() {}
		virtual void update() {}
		virtual void render() {}

		inline const std::string& getName() const { return m_name; }

	protected: /* functions */

		// Can be used by derived classes to stop running the application.
		// NOTE: This function is needed because derived classes cannot directly call m_application's stopRunning() function because it's private.
		//       This Layer class here is a friend class to PekanApplication, that's why it CAN call stopRunning().
		void stopRunningApplication();

	protected: /* variables */

		// Application containing this layer
		PekanApplication* m_application = nullptr;

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