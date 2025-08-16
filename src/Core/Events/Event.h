#pragma once

#include <ostream>
#include <string>
#include <functional>
#include <queue>
#include <memory>

struct GLFWwindow;

namespace Pekan
{

	enum class EventType
	{
		None = 0,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		WindowClosed, WindowResized
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryInput = 1 << 0,
		EventCategoryKeyboard = 1 << 1,
		EventCategoryMouse = 1 << 2,
		EventCategoryMouseButton = 1 << 3,
		EventCategoryWindow = 1 << 4
	};

#define EVENT_CLASS_TYPE(type) \
	static EventType getStaticType() { return EventType::##type; } \
	virtual EventType getEventType() const override { return getStaticType(); } \
	virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
	virtual int getCategoryFlags() const override { return category; }

	// A base class for all events in Pekan
	class Event
	{
	public:
		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }

		inline bool isInCategory(EventCategory category)
		{
			return getCategoryFlags() & category;
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}

	// A queue of events
	// where events are pushed when they happen
	// and can be handled later, usually once per frame,
	// by popping them one by one from the queue.
	class EventQueue
	{
	public:

		// Pushes an event to the event queue
		void push(std::unique_ptr<Event> event)
		{
			m_queue.push(std::move(event));
		}

		// Checks if the event queue is empty
		bool empty() const
		{
			return m_queue.empty();
		}

		// Returns event queue's size
		int size() const
		{
			return m_queue.size();
		}

		// Pops an event from the back of the event queue.
		// Returns the popped event.
		std::unique_ptr<Event> pop()
		{
			std::unique_ptr<Event> event = std::move(m_queue.front());
			m_queue.pop();
			return event;
		}

	private:

		// Underlying queue of event pointers
		std::queue<std::unique_ptr<Event>> m_queue;
	};

} // namespace Pekan
