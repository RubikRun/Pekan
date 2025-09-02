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

	// A base class for a layer in Pekan.
	// A layer in Pekan is responsible for rendering a part of an application, updating itself between frames.
	// Layers in Pekan are usually composed together in a LayerStack which makes up the visuals of an application.
	// ----
	// For example, in a 3D game we might have a layer for the game world, and another layer for the GUI.
	// We'd want the game world layer to be rendered first, and then on top of it render the GUI layer.
	class Layer : public EventListener
	{
		// We need LayerStack as a friend class,
		// because it is reponsible for initializing and exiting layers,
		// and we don't want any other code to be able to initialize and exit layers.
		friend class LayerStack;

	public:

		Layer(PekanApplication* application) : m_application(application) {}
		virtual ~Layer() = default;

		// Can be implemented by derived classes with specific logic for updating the layer between frames
		virtual void update(double deltaTime) {}
		// Can be implemented by derived classes with specific rendering logic
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

	private: /* functions */

		// Can be implemented by derived classes with specific initialization logic
		virtual bool init() { return true; }
		// Can be implemented by derived classes with specific exiting logic
		virtual void exit() {}

	private: /* variables*/

		// Pointer to layer's parent.
		// A layer depends on its parent, meaning that its parent must be initialized first.
		Layer_Ptr m_parent;

		// A flag indicating if layer has been initialized and not yet exited
		bool m_isInitialized = false;
	};

} // namespace Pekan