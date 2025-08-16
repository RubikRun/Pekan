#pragma once

#include "Event.h"
#include "MouseEvents_Enums.h"

#include <sstream>

namespace Pekan
{

	// A type of event that occurs when the mouse is moved
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_mouseX(x), m_mouseY(y)
		{}

		inline float getX() const { return m_mouseX; }
		inline float getY() const { return m_mouseY; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent { mouseX: " << m_mouseX << ", mouseY: " << m_mouseY << " }";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_mouseX, m_mouseY;
	};

	// A type of event that occurs when the scroll wheel of the mouse is scrolled
	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_xOffset(xOffset), m_yOffset(yOffset)
		{}

		inline float getXOffset() const { return m_xOffset; }
		inline float getYOffset() const { return m_yOffset; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent { xOffset: " << getXOffset() << ", yOffset: " << getYOffset() << " }";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_xOffset, m_yOffset;
	};

	// A type of event related to a button of the mouse
	class MouseButtonEvent : public Event
	{
	public:
		inline MouseButton getMouseButton() const { return m_button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(MouseButton button)
			: m_button(button)
		{}

		MouseButton m_button;
	};

	// A type of event that occurs when a mouse button is pressed
	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(MouseButton button)
			: MouseButtonEvent(button)
		{}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent { button: " << int(m_button) << " }";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	// A type of event that occurs when a mouse button is released, after being pressed
	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(MouseButton button)
			: MouseButtonEvent(button)
		{}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent { button: " << int(m_button) << " }";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

} // namespace Pekan