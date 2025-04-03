#pragma once

#include "Event.h"

#include <sstream>

namespace Pekan
{

	class KeyEvent : public Event
	{
	public:
		inline int getKeyCode() const { return m_keyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keyCode)
			: m_keyCode(keyCode) {
		}

		int m_keyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, bool isRepeating)
			: KeyEvent(keyCode), m_isRepeating(isRepeating)
		{}

		inline bool isRepeating() const { return m_isRepeating; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent { keyCode: " << m_keyCode << ", isRepeating: " << m_isRepeating << " }";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_isRepeating;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode)
		{}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent { keyCode: " << m_keyCode << " }";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

} // namespace Pekan