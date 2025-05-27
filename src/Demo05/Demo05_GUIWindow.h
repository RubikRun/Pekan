#pragma once

#include "PekanGUIWindow.h"
using Pekan::PekanGUIWindow;

#include "imgui.h"

namespace Demo
{

	class Demo05_GUIWindow : public PekanGUIWindow
	{
	public:

		Demo05_GUIWindow(Pekan::PekanApplication* application) : PekanGUIWindow(application) {}

		// Returns current value of clear color
		inline const ImVec4& getClearColor() const { return m_clearColor; }

		// Returns current line thickness
		inline float getLineThickness() const { return m_lineThickness; }

	private: /* functions */

		void _render() override;

		bool init() override;

	private: /* variables */

		// Color for clearing scene's background
		ImVec4 m_clearColor = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

		// Line thickness to be used for MTT's lines in Demo05
		float m_lineThickness = 0.001f;
	};

} // namespace Demo