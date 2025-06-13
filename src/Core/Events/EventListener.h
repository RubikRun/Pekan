#pragma once

namespace Pekan
{
	
	class PekanApplication;

	class KeyPressedEvent; class KeyReleasedEvent;
	class MouseMovedEvent; class MouseScrolledEvent; class MouseButtonPressedEvent; class MouseButtonReleasedEvent;
	class WindowResizedEvent; class WindowClosedEvent;

	// A base class used for receiving events from Pekan.
	// Each class derived from EventListener must be registered in a PekanApplication.
	// When an event occurs in an application it is automatically sent to all registered EventListeners.
	class EventListener
	{
		friend class PekanApplication;


	protected: // TODO: find out why not protected




		// Functions that are automatically called when an event occurs in an application.
		// Each of these functions can be overriden by a derived class to handle the specific type of event when it occurs.
		// If an event is successfully handled in one of these functions, then that function must return true
		// to indicate to Pekan that the event is handled.
		virtual bool onKeyPressed(KeyPressedEvent& event) { return false; }
		virtual bool onKeyReleased(KeyReleasedEvent& event) { return false; }
		virtual bool onMouseMoved(MouseMovedEvent& event) { return false; }
		virtual bool onMouseScrolled(MouseScrolledEvent& event) { return false; }
		virtual bool onMouseButtonPressed(MouseButtonPressedEvent& event) { return false; }
		virtual bool onMouseButtonReleased(MouseButtonReleasedEvent& event) { return false; }
		virtual bool onWindowResized(WindowResizedEvent& event) { return false; }
		virtual bool onWindowClosed(WindowClosedEvent& event) { return false; }
	};

} // namespace Pekan