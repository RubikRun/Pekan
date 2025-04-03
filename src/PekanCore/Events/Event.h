#pragma once

#include <GLFW/glfw3.h>

#include <string>
#include <functional>

namespace Pekan
{

	class KeyPressedEvent; class KeyReleasedEvent;
	class MouseMovedEvent; class MouseScrolledEvent; class MouseButtonPressedEvent; class MouseButtonReleasedEvent;
	class WindowResizedEvent; class WindowClosedEvent;

	typedef std::function<bool(KeyPressedEvent&)> KeyPressedCallback;
	typedef std::function<bool(KeyReleasedEvent&)> KeyReleasedCallback;
	typedef std::function<bool(MouseMovedEvent&)> MouseMovedCallback;
	typedef std::function<bool(MouseScrolledEvent&)> MouseScrolledCallback;
	typedef std::function<bool(MouseButtonPressedEvent&)> MouseButtonPressedCallback;
	typedef std::function<bool(MouseButtonReleasedEvent&)> MouseButtonReleasedCallback;
	typedef std::function<bool(WindowResizedEvent&)> WindowResizedCallback;
	typedef std::function<bool(WindowClosedEvent&)> WindowClosedCallback;

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
		friend class EventHandler;

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

	class EventHandler
	{
		friend class PekanEngine;

	public:
		static inline void registerKeyPressedCallback(const KeyPressedCallback& callback) { s_keyPressedCallbacks.push_back(callback); }
		static inline void registerKeyReleasedCallback(const KeyReleasedCallback& callback) { s_keyReleasedCallbacks.push_back(callback); }
		static inline void registerMouseMovedCallback(const MouseMovedCallback& callback) { s_mouseMovedCallbacks.push_back(callback); }
		static inline void registerMouseScrolledCallback(const MouseScrolledCallback& callback) { s_mouseScrolledCallbacks.push_back(callback); }
		static inline void registerMouseButtonPressedCallback(const MouseButtonPressedCallback& callback) { s_mouseButtonPressedCallbacks.push_back(callback); }
		static inline void registerMouseButtonReleasedCallback(const MouseButtonReleasedCallback& callback) { s_mouseButtonReleasedCallbacks.push_back(callback); }
		static inline void registerWindowResizedCallback(const WindowResizedCallback& callback) { s_windowResizedCallbacks.push_back(callback); }
		static inline void registerWindowClosedCallback(const WindowClosedCallback& callback) { s_windowClosedCallbacks.push_back(callback); }

	private: /* functions */
		static void handleKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void handleMouseMovedEvent(GLFWwindow* window, double xPos, double yPos);
		static void handleMouseScrolledEvent(GLFWwindow* window, double xOffset, double yOffset);
		static void handleMouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
		static void handleWindowResizedEvent(GLFWwindow* window, int width, int height);
		static void handleWindowClosedEvent(GLFWwindow* window);

		template <typename EventT, typename CallbackT>
		static void handleEvent(EventT& event, const std::vector<CallbackT>& callbacks)
		{
			for (const CallbackT& callback : callbacks)
			{
				if (callback(event))
				{
					event.m_handled = true;
					break;
				}
			}
		}

	private: /* variables */
		static std::vector<KeyPressedCallback> s_keyPressedCallbacks;
		static std::vector<KeyReleasedCallback> s_keyReleasedCallbacks;
		static std::vector<MouseMovedCallback> s_mouseMovedCallbacks;
		static std::vector<MouseScrolledCallback> s_mouseScrolledCallbacks;
		static std::vector<MouseButtonPressedCallback> s_mouseButtonPressedCallbacks;
		static std::vector<MouseButtonReleasedCallback> s_mouseButtonReleasedCallbacks;
		static std::vector<WindowResizedCallback> s_windowResizedCallbacks;
		static std::vector<WindowClosedCallback> s_windowClosedCallbacks;
	};

} // namespace Pekan
