#pragma once

#include "PekanGUIWindow.h"
using Pekan::PekanGUIWindow;

#include "imgui.h"

namespace Demo
{

	class Demo00_GUIWindow : public PekanGUIWindow
	{
	public:

		// Returns current value of clear color
		inline const ImVec4& getClearColor() const { return clearColor; }

		// Returns current value of position
		inline const ImVec2& getPosition() const { return position; }

	private: /* functions */

		void _render() override;

		bool _init() override;

	private: /* variables */

		// Color for clearing scene's background
		ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		// Position of the square, relative to window's center
		ImVec2 position = ImVec2(0.0f, 0.0f);
	};

} // namespace Demo