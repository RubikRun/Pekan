#pragma once

#include "PekanApplication.h"
using Pekan::PekanApplication;

namespace Demo
{

	class Demo02_Application : public PekanApplication
	{
		bool _init() override;
		std::string getName() const override { return "Rotating Cube"; }
	};

} // namespace Demo