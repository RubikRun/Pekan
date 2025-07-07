#pragma once

#include "PekanApplication.h"
using Pekan::PekanApplication;

namespace Demo
{

	class Demo01_Application : public PekanApplication
	{
		bool _init(Pekan::LayerStack& layerStack) override;
		std::string getName() const override { return "Squares Editor"; }
		Pekan::ApplicationProperties getProperties() const override;
	};

} // namespace Demo