#pragma once

#include "PekanApplication.h"

namespace Demo
{

	class Demo05_Application : public Pekan::PekanApplication
	{
		bool _init(Pekan::LayerStack& layerStack) override;
		std::string getName() const override { return "Mandelbrot Times Table"; }
	};

} // namespace Demo