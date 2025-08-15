#pragma once

#include "Layer.h"
#include <vector>

namespace Pekan
{

	// A stack of layers,
	// used for storing the layers of an application in a specific order.
	// Allows layers to be traversed forwards (for rendering) and backwards (for events).
	class LayerStack
	{
	public:

		// Pushes a layer to the end of the layer stack
		void pushLayer(const Layer_Ptr& layer);

		// Initializes all layers of the layer stack
		void initAll();
		// Exits all layers of the layer stack
		void exitAll();

		// Renders all layers, in the order they were pushed to the layer stack
		void renderAll();
		// Updates all layers, in the order they were pushed to the layer stack
		void updateAll(double deltaTime);

		// Sends an event of a given type to layers of the layer stack,
		// one by one, until a layer successfully handles the event,
		// in order opposite to how they were pushed to the layer stack.
		//
		// @return true if event was successfully handled by a layer.
		template<typename EventT>
		bool dispatchEvent
		(
			std::unique_ptr<EventT>& event,
			bool (EventListener::* onEventFunc)(const EventT&)
		);

	private: /* functions */

		// Initializes a given layer,
		// recursively making sure that parent is initialized first.
		void initLayer(const Layer_Ptr& layer);

	private: /* variables */

		// A list of layers making up the layer stack
		std::vector<Layer_Ptr> m_layers;
	};

	template<typename EventT>
	bool LayerStack::dispatchEvent
	(
		std::unique_ptr<EventT>& event,
		bool (EventListener::* onEventFunc)(const EventT&)
	)
	{
		for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
		{
			EventListener* layer = static_cast<EventListener*>((*it).get());
			if (layer != nullptr && (layer->*onEventFunc)(*event.get()))
			{
				return true;
			}
		}
		return false;
	}

} // namespace Pekan