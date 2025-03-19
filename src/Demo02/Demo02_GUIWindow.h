#pragma once

#include "PekanGUIWindow.h"
using Pekan::PekanGUIWindow;

namespace Demo
{

	class Demo02_GUIWindow : public PekanGUIWindow
	{
	private: /* functions */

		void _render() override;

		bool init() override;
	};

} // namespace Demo