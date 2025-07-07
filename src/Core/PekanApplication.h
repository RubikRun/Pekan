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
	class Layer;

	// An application's properties, grouped together in a struct
	struct ApplicationProperties
	{
		// Properties of the window where application will run
		WindowProperties windowProperties;

		// Target frames per second.
		// Value of 0.0 means use FPS equal to monitor's refresh rate (VSync)
		double fps = 0.0;
	};

	// A base class for all Pekan applications
	class PekanApplication
	{
		friend class PekanEngine;
		friend class Window;
		friend class Layer;

	public:

		virtual ~PekanApplication() { exit(); }

		bool init();

		void run();

		void exit();

		virtual std::string getName() const { return ""; }

		void registerEventListener(const std::shared_ptr<EventListener>& eventListener);
		void unregisterEventListener(const std::shared_ptr<EventListener>& eventListener);

	private: /* functions */

		// Initializes the application.
		// 
		// To be implemented by derived classes to fill layer stack with application's layers.
		virtual bool _init(LayerStack& layerStack) = 0;

		// Can be overriden by derived classes to return specific application properties.
		// If not overriden, default application properties will be used.
		virtual ApplicationProperties getProperties() const { return {}; }

		// Stops running the main loop and closes the window
		void stopRunning();

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
		//
		// Can be implemented by derived classes with specific logic of handling the events from the event queue.
		virtual void handleEventQueue() { while (!m_eventQueue.empty()) { m_eventQueue.pop(); } }

	private: /* variables */

		// Stack of layers making up the application
		LayerStack m_layerStack;

		// Event queue where events are pushed if they are not handled by any layer
		EventQueue m_eventQueue;

		// List of registered event listeners that need to be notified when an event occurs
		std::vector<std::weak_ptr<EventListener>> m_eventListeners;

		// Delta timer used to keep track of time passed since last frame was rendered
		DeltaTimer m_deltaTimer;
	};

} // namespace Pekan