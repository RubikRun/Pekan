#pragma once

#include "PekanApplication.h"

namespace Pekan
{
namespace Editor
{

	class EditorApplication : public Pekan::PekanApplication
	{
		bool _fillLayerStack(Pekan::LayerStack& layerStack) override;
		std::string getName() const override { return "Pekan Editor"; }
		Pekan::ApplicationProperties getProperties() const override;
	};

} // namespace Editor
} // namespace Pekan
