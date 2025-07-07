#pragma once

#include "PekanApplication.h"

namespace Demo
{

	class Demo04_Application : public Pekan::PekanApplication
	{
		bool _init(Pekan::LayerStack& layerStack) override;
		std::string getName() const override { return "Textures & Shapes"; }
		Pekan::ApplicationProperties getProperties() const override;
	};

} // namespace Demo