#pragma once

#include "PekanApplication.h"
using Pekan::PekanApplication;

namespace Demo
{

	class Demo00_Application : public PekanApplication
	{
		bool _init() override;
	};

} // namespace Demo