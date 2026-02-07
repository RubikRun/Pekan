#pragma once

#include "PekanApplication.h"

namespace Demo
{

	class Demo10_Application : public Pekan::PekanApplication
	{
		bool _fillLayerStack(Pekan::LayerStack& layerStack) override;
		std::string getName() const override { return "Transform Hierarchies"; }
		Pekan::ApplicationProperties getProperties() const override;
	};

} // namespace Demo
