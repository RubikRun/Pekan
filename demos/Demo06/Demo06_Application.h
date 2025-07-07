#pragma once

#include "PekanApplication.h"

namespace Demo
{

	class Demo06_Application : public Pekan::PekanApplication
	{
		bool _init(Pekan::LayerStack& layerStack) override;
		std::string getName() const override { return "Random Shapes"; }
		Pekan::ApplicationProperties getProperties() const override;
	};

} // namespace Demo