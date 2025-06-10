#pragma once

#include "PekanApplication.h"

namespace Demo
{

	class Demo06_Application : public Pekan::PekanApplication
	{
		bool _init(Pekan::LayerStack& layerStack) override;
		std::string getName() const override { return "Rectangle Gradient"; }
	};

} // namespace Demo