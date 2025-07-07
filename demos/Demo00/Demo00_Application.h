#pragma once

#include "PekanApplication.h"

namespace Demo
{

	class Demo00_Application : public Pekan::PekanApplication
	{
		bool _fillLayerStack(Pekan::LayerStack& layerStack) override;
		std::string getName() const override { return "Rectangle Gradient"; }
		Pekan::ApplicationProperties getProperties() const override;
	};

} // namespace Demo