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

		void pushLayer(const LayerPtr& layer);
		void popLayer(const LayerPtr& layer);

		// Iterators for traversing the layers stack forwards
		std::vector<LayerPtr>::iterator begin() { return m_layers.begin(); }
		std::vector<LayerPtr>::iterator end() { return m_layers.end(); }
		// Iterators for traversing the layers stack backwards
		std::vector<LayerPtr>::reverse_iterator rbegin() { return m_layers.rbegin(); }
		std::vector<LayerPtr>::reverse_iterator rend() { return m_layers.rend(); }

	private:

		// A list of layers making up the layer stack
		std::vector<LayerPtr> m_layers;
	};

} // namespace Pekan