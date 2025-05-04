#pragma once

#include "Events/Event.h"
#include "Events/KeyEvent_Enums.h"
#include "Events/MouseEvent_Enums.h"
#include "LayerStack.h"
#include "Time/DeltaTimer.h"

#include <string>

namespace Pekan
{
	class Window;
	class Layer;

	// A base class for all Pekan applications
	class PekanApplication
	{
		friend class Window;
		friend class Layer;

	public:

		virtual ~PekanApplication() { exit(); }

		bool init();

		void run();

		void exit();

		virtual std::string getName() const { return ""; }

	protected: /* functions */

		// Set FPS (frames per second) to be used for running the application.
		// If you don't manually set FPS your application will run with FPS equal to monitor's refresh rate (VSync)
		inline void setFPS(double fps) { m_fps = fps; }

	private: /* functions */

		// Initializes the application.
		// 
		// To be implemented by derived classes
		// to set up application's layer stack
		virtual bool _init() = 0;

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

	protected: /* variables */

		// Stack of layers making up the application
		LayerStack m_layerStack;

		// Frames per second.
		// Value of 0.0 means use FPS equal to monitor's refresh rate (VSync)
		double m_fps = 0.0;

		// Event queue where events are pushed if they are not handled by any layer
		EventQueue m_eventQueue;

		// Delta timer used to keep track of time passed since last frame was rendered
		DeltaTimer m_deltaTimer;
	};

} // namespace Pekan