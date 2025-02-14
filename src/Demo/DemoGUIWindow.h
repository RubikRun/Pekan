#pragma once

#include "PekanGUIWindow.h"
using Pekan::PekanGUIWindow;

#include "imgui.h"

namespace Demo
{

	class DemoGUIWindow : public PekanGUIWindow
	{
	public:

		// Returns current value of clear color
		const ImVec4& getClearColor() const { return clearColor; }

	private: /* functions */

		void _render() override;

		bool _init() override;

	private: /* variables */

		// Color for clearing scene's background
		ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	};

} // namespace Demo