#pragma once

#include "PekanApplication.h"

namespace Demo
{

	class Demo07_Application : public Pekan::PekanApplication
	{
		bool _init(Pekan::LayerStack& layerStack) override;
		std::string getName() const override { return "Shapes Transform"; }
	};

} // namespace Demo