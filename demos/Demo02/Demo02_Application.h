#pragma once

#include "PekanApplication.h"
using Pekan::PekanApplication;

namespace Demo
{

	class Demo02_Application : public PekanApplication
	{
		bool _fillLayerStack(Pekan::LayerStack& layerStack) override;
		std::string getName() const override { return "Rotating Cube"; }
		Pekan::ApplicationProperties getProperties() const override;
	};

} // namespace Demo