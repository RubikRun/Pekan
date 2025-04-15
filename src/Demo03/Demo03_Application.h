#pragma once

#include "PekanApplication.h"
using Pekan::PekanApplication;

namespace Demo
{

	class Demo03_Application : public PekanApplication
	{
		bool _init() override;
		std::string getName() const override { return "Snake Game"; }
	};

} // namespace Demo