#pragma once

#include <LayerStack.h>
#include <string>

namespace Pekan
{
	class PekanEngine;

	// A base class for all Pekan applications
	class PekanApplication
	{
		friend class PekanEngine;

	public:

		virtual ~PekanApplication() { exit(); }

		bool init();

		void run();

		void exit();

		virtual std::string getName() const { return ""; }

	private: /* functions */

		// Initializes the application.
		// 
		// To be implemented by derived classes
		// to set up application's layer stack
		virtual bool _init() = 0;

		// Functions that are called when an event occurs.
		// Each of these functions handles a specific type of event
		// by sending it to each layer of the application, one by one, until a layer succesfully handles the event.
		// The order of layers receiving the event is the opposite of the order of rendering,
		// meaning that layers drawn last (on top) receive events first.
		void handleKeyEvent(int key, int scancode, int action, int mods);
		void handleMouseMovedEvent(double xPos, double yPos);
		void handleMouseScrolledEvent(double xOffset, double yOffset);
		void handleMouseButtonEvent(int button, int action, int mods);
		void handleWindowResizedEvent(int width, int height);
		void handleWindowClosedEvent();

	protected: /* variables */

		// Stack of layers making up the application
		LayerStack m_layerStack;

		// Flag indicating whether application should be rendered at full screen.
		// To be set by derived classes inside of the _init() function.
		bool m_isFullScreen = false;

		// Flag indicating whether mouse's cursor should be hidden.
		// To be set by derived classes inside of the _init() function.
		bool m_shouldHideCursor = false;
	};

} // namespace Pekan