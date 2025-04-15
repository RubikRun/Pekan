#pragma once

#include "PekanApplication.h"

namespace Demo
{

	class Demo00_Application : public Pekan::PekanApplication
	{
		bool _init() override;
		std::string getName() const override { return "Rectangle Gradient"; }
	};

} // namespace Demo