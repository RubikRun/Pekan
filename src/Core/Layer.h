#pragma once

#include "Events/EventListener.h"
#include <string>

namespace Pekan
{

	class PekanApplication;

	class Layer : public EventListener
	{
		friend class PekanApplication;

	public:
		Layer(const std::string& name, PekanApplication* application) : m_name(name), m_application(application) {}
		virtual ~Layer() = default;

		virtual bool init() { return true; }
		virtual void exit() {}
		virtual void update(double deltaTime) {}
		virtual void render() {}

		inline const std::string& getName() const { return m_name; }

	protected: /* functions */

		// Can be used by derived classes to stop running the application.
		// NOTE: This function is needed because derived classes cannot directly call m_application's stopRunning() function because it's private.
		//       This Layer class here is a friend class to PekanApplication, that's why it CAN call stopRunning().
		void stopRunningApplication();

	protected: /* variables */

		// Application containing this layer
		PekanApplication* m_application = nullptr;

	private: /* variables*/

		std::string m_name;
	};

} // namespace Pekan