#pragma once

#include "PekanGUIWindow.h"
using Pekan::PekanGUIWindow;

#include "imgui.h"

namespace Demo
{

	class Demo07_GUIWindow : public PekanGUIWindow
	{
	public:

		Demo07_GUIWindow(Pekan::PekanApplication* application) : PekanGUIWindow(application) {}

	private: /* functions */

		void _render() override;

		bool init() override;

	private: /* variables */
	};

} // namespace Demo