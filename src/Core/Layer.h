#pragma once

#include "Events/EventListener.h"
#include <string>

#include <memory>

namespace Pekan
{

	class Layer;
	class LayerStack;

	typedef std::shared_ptr<Layer> Layer_Ptr;
	typedef std::shared_ptr<const Layer> Layer_ConstPtr;

	class Layer : public EventListener
	{
		friend class LayerStack;

	public:

		Layer(PekanApplication* application) : m_application(application) {}
		virtual ~Layer() = default;

		virtual bool init() { return true; }
		virtual void exit() {}
		virtual void update(double deltaTime) {}
		virtual void render() const {}

		// To be implemented by derived classes to return layer's name
		virtual std::string getLayerName() const = 0;

		// Sets layer's parent.
		// A layer depends on its parent, meaning that its parent must be initialized first.
		// If layer does NOT depend on any other layer you don't need to set a parent.
		void setParent(const Layer_Ptr& parent) { m_parent = parent; }

	protected: /* variables */

		// Application containing this layer
		PekanApplication* m_application = nullptr;

	private: /* variables*/

		// Pointer to layer's parent.
		// A layer depends on its parent, meaning that its parent must be initialized first.
		Layer_Ptr m_parent;

		bool m_isInitialized = false;
	};

} // namespace Pekan