#pragma once

#include <string>
#include <functional>

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

	class Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }

		inline bool isInCategory(EventCategory category)
		{
			return getCategoryFlags() & category;
		}
	protected:
		bool m_handled = false;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}

} // namespace Pekan
