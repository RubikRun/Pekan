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

		void pushLayer(const Layer_Ptr& layer);
		void popLayer(const Layer_Ptr& layer);

		// Initializes all layers of the layer stack
		void initAll();
		// Exits all layers of the layer stack
		void exitAll();

		// Iterators for traversing the layers stack forwards
		std::vector<Layer_Ptr>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer_Ptr>::iterator end() { return m_layers.end(); }
		// Iterators for traversing the layers stack backwards
		std::vector<Layer_Ptr>::reverse_iterator rbegin() { return m_layers.rbegin(); }
		std::vector<Layer_Ptr>::reverse_iterator rend() { return m_layers.rend(); }

	private: /* functions */

		// Initializes a given layer,
		// recursively making sure that parent is initialized first.
		void initLayer(const Layer_Ptr& layer);

	private: /* variables */

		// A list of layers making up the layer stack
		std::vector<Layer_Ptr> m_layers;
	};

} // namespace Pekan