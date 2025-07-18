#include "LayerStack.h"

namespace Pekan
{

	void LayerStack::pushLayer(const Layer_Ptr& layer)
	{
		m_layers.push_back(layer);
	}

	void LayerStack::popLayer(const Layer_Ptr& layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
		}
	}

} // namespace Pekan