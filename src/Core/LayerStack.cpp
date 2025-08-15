#include "LayerStack.h"

#include "PekanLogger.h"

namespace Pekan
{

	void LayerStack::pushLayer(const Layer_Ptr& layer)
	{
		m_layers.push_back(layer);
	}

	void LayerStack::initAll()
	{
		for (const Layer_Ptr& layer : m_layers)
		{
			initLayer(layer);
		}
	}

	void LayerStack::exitAll()
	{
		for (const Layer_Ptr& layer : m_layers)
		{
			PK_ASSERT_QUICK(layer != nullptr);
			if (layer != nullptr)
			{
				layer->exit();
				layer->m_isInitialized = false;
			}
		}
	}

	void LayerStack::renderAll()
	{
		for (Layer_Ptr layer : m_layers)
		{
			if (layer != nullptr)
			{
				layer->render();
			}
		}
	}

	void LayerStack::updateAll(double deltaTime)
	{
		for (Layer_Ptr layer : m_layers)
		{
			if (layer != nullptr)
			{
				layer->update(deltaTime);
			}
		}
	}

	void LayerStack::initLayer(const Layer_Ptr& layer)
	{
		PK_ASSERT_QUICK(layer != nullptr);
		if (layer->m_isInitialized)
		{
			return;
		}
		// If layer has a parent that is not yet initialized, initialize the parent first.
		if (layer->m_parent != nullptr && !layer->m_parent->m_isInitialized)
		{
			initLayer(layer->m_parent);
		}
		// Initialize layer
		layer->m_isInitialized = layer->init();
		if (!layer->m_isInitialized)
		{
			PK_LOG_ERROR("Failed to initialize layer \"" << layer->getLayerName() << "\".", "Pekan");
		}
	}

} // namespace Pekan