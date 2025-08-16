#pragma once

#include "Event.h"
#include "KeyEvents_Enums.h"

#include <sstream>

namespace Pekan
{

	// A type of event related to a key of the keyboard
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

	// A type of event that occurs when a key of the keyboard is pressed
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

	// A type of event that occurs when a key of the keyboard is released, after being pressed
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