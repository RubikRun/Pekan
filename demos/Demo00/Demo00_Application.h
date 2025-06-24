#pragma once

#include "PekanApplication.h"

namespace Demo
{

	class Demo00_Application : public Pekan::PekanApplication
	{
		bool _init(Pekan::LayerStack& layerStack) override;
		std::string getName() const override { return "Rectangle Gradient"; }
	};

} // namespace Demo