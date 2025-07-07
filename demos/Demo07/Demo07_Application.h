#pragma once

#include "PekanApplication.h"

namespace Demo
{

	class Demo07_Application : public Pekan::PekanApplication
	{
		bool _fillLayerStack(Pekan::LayerStack& layerStack) override;
		std::string getName() const override { return "Shapes Transform"; }
		Pekan::ApplicationProperties getProperties() const override;
	};

} // namespace Demo