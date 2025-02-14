#pragma once

#include "PekanApplication.h"
using Pekan::PekanApplication;

namespace Demo
{

	class DemoApplication : public PekanApplication
	{
		bool _init() override;
	};

} // namespace Demo