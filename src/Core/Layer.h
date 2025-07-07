#pragma once

#include "Events/EventListener.h"
#include <string>

#include <memory>

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

	protected: /* variables */

		// Application containing this layer
		PekanApplication* m_application = nullptr;

	private: /* variables*/

		std::string m_name;
	};

	typedef std::shared_ptr<Layer> LayerPtr;

} // namespace Pekan