#pragma once

#include "Layer.h"

#include "Snake.h"
#include "Apple.h"

// TEMP: testing event system
namespace Pekan
{
	class KeyPressedEvent; class KeyReleasedEvent;
	class MouseMovedEvent; class MouseScrolledEvent; class MouseButtonPressedEvent; class MouseButtonReleasedEvent;
	class WindowResizedEvent; class WindowClosedEvent;
}

namespace Demo
{

	class Demo03_Scene : public Pekan::Layer
	{
	public:

		Demo03_Scene() : Layer("scene_layer") {}

		bool init() override;

		void update() override;

		void render() override;

		void exit() override;

	private: /* functions */

		// TEMP: testing event system
		bool onKeyPressed(Pekan::KeyPressedEvent& event) override;
		bool onKeyReleased(Pekan::KeyReleasedEvent& event) override;
		bool onMouseMoved(Pekan::MouseMovedEvent& event) override;
		bool onMouseScrolled(Pekan::MouseScrolledEvent& event) override;
		bool onMouseButtonPressed(Pekan::MouseButtonPressedEvent& event) override;
		bool onMouseButtonReleased(Pekan::MouseButtonReleasedEvent& event) override;
		bool onWindowResized(Pekan::WindowResizedEvent& event) override;
		bool onWindowClosed(Pekan::WindowClosedEvent& event) override;

	private: /* variables */

		Snake m_snake;

		Apple m_apple;
	};

} // namespace Demo