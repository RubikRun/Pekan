#pragma once

#include "PekanApplication.h"

namespace Demo
{

	class DemoIg00_Application : public Pekan::PekanApplication
	{
		bool _fillLayerStack(Pekan::LayerStack& layerStack) override;
		std::string getName() const override { return "2D Platformer"; }
		Pekan::ApplicationProperties getProperties() const override;
	};

} // namespace Demo
