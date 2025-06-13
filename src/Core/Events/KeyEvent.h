#pragma once

#include "Event.h"
#include "KeyEvent_Enums.h"

#include <sstream>

namespace Pekan
{

	class KeyEvent : public Event
	{
	public:
		inline KeyCode getKeyCode() const { return m_keyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(KeyCode keyCode)
			: m_keyCode(keyCode) {
		}

		KeyCode m_keyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keyCode, bool isRepeating)
			: KeyEvent(keyCode), m_isRepeating(isRepeating)
		{}

		inline bool isRepeating() const { return m_isRepeating; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent { keyCode: " << int(m_keyCode) << ", isRepeating: " << m_isRepeating << " }";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_isRepeating;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keyCode)
			: KeyEvent(keyCode)
		{}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent { keyCode: " << int(m_keyCode) << " }";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

} // namespace Pekan