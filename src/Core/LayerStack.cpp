#include "LayerStack.h"

namespace Pekan
{

	void LayerStack::pushLayer(const LayerPtr& layer)
	{
		m_layers.push_back(layer);
	}

	void LayerStack::popLayer(const LayerPtr& layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
		}
	}

} // namespace Pekan