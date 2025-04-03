#pragma once

#include "Event.h"

#include <sstream>

namespace Pekan
{

	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(int width, int height)
			: m_width(width), m_height(height)
		{}

		inline unsigned int getWidth() const { return m_width; }
		inline unsigned int getHeight() const { return m_height; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "WindowResizedEvent { width: " << m_width << ", height: " << m_height << " }";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResized)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	private:
		unsigned int m_width, m_height;
	};

	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "WindowClosedEvent {}";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowClosed)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	};

} // namespace Pekan