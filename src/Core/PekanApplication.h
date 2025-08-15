#pragma once

#include "Events/Event.h"
#include "Events/EventListener.h"
#include "LayerStack.h"
#include "Time/DeltaTimer.h"
#include "Window.h"

#include <string>
#include <memory>

namespace Pekan
{

	// Properties of a Pekan application, grouped together in a struct
	struct ApplicationProperties
	{
		// Properties of the window where application will run
		WindowProperties windowProperties;

		// Target FPS (frames per second)
		double fps = 0.0;

		// Flag indicating if application should use VSync,
		// meaning to target FPS equal to monitor's refresh rate.
		//
		// NOTE: Has effect only if there is no target FPS set
		bool useVSync = true;

		// Number of samples per pixel to be used for multisampling
		int numberOfSamples = 1;
	};

	// A base class for all Pekan applications
	class PekanApplication
	{
		// We need class Window as a friend class
		// because Window needs to send events to PekanApplication
		// using the handleKeyEvent(), handleMouseMovedEvent(), etc. functions,
		// and these functions are private because we don't want anyone else to be able to call them.
		friend class Window;

	public:

		virtual ~PekanApplication() { exit(); }

		bool init();

		void run();

		void exit();

		virtual std::string getName() const { return ""; }

		// Registers an event listener to be notified when an event occurs in this application
		void registerEventListener(const std::shared_ptr<EventListener>& eventListener);
		// Unregisters an event listener. It will no longer be notified when an event occurs in this application.
		void unregisterEventListener(const std::shared_ptr<EventListener>& eventListener);

		// Can be overriden by derived classes to return specific application properties.
		// If not overriden, default application properties will be used.
		virtual ApplicationProperties getProperties() const { return {}; }

		// Stops running the application, and closes the window.
		void stopRunning();

		// Checks if application is valid, meaning that it has been initialized and not yet exited
		bool isValid() const { return m_isInitialized; }

	private: /* functions */

		// Can be implemented by derived classes with specific initialization logic.
		// @return true on success
		virtual bool _init() { return true; }

		// To be implemented by derived classes to fill layer stack with application's layers.
		// @return true on success
		virtual bool _fillLayerStack(LayerStack& layerStack) = 0;

		// Functions that are called when an event occurs.
		// Each of these functions handles a specific type of event
		// by sending it to each layer of the application, one by one, until a layer succesfully handles the event.
		// The order of layers receiving the event is the opposite of the order of rendering,
		// meaning that layers drawn last (on top) receive events first.
		void handleKeyEvent(KeyCode key, int scancode, int action, int mods);
		void handleMouseMovedEvent(double xPos, double yPos);
		void handleMouseScrolledEvent(double xOffset, double yOffset);
		void handleMouseButtonEvent(MouseButton button, int action, int mods);
		void handleWindowResizedEvent(int width, int height);
		void handleWindowClosedEvent();

		// Handles the event queue.
		// The event queue is a queue of left-over events that were not handled by any layer or any event listener.
		//
		// Can be implemented by derived classes with specific logic of handling the events from the event queue.
		// NOTE: Make sure to pop all events from the queue, otherwise they will keep piling up.
		virtual void handleEventQueue() { while (!m_eventQueue.empty()) { m_eventQueue.pop(); } }

	private: /* variables */

		// Stack of layers making up the application
		LayerStack m_layerStack;

		// Event queue where events are pushed if they are not handled by any layer or any event listener.
		EventQueue m_eventQueue;

		// List of registered event listeners that need to be notified when an event occurs
		std::vector<std::weak_ptr<EventListener>> m_eventListeners;

		// Delta timer used to keep track of time passed since last frame was rendered
		DeltaTimer m_deltaTimer;

		// A flag indicating if application has been initialized and not yet exited
		bool m_isInitialized = false;
	};

} // namespace Pekan