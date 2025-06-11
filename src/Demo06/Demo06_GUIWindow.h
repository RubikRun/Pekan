#pragma once

#include "PekanGUIWindow.h"
using Pekan::PekanGUIWindow;

#include "imgui.h"

namespace Demo
{

	class Demo06_GUIWindow : public PekanGUIWindow
	{
	public:

		Demo06_GUIWindow(Pekan::PekanApplication* application) : PekanGUIWindow(application) {}

		// Returns current number of shapes
		inline int getNumberOfShapes() const { return m_numberOfShapes; }

	private: /* functions */

		void _render() override;

		bool init() override;

	private: /* variables */

		int m_numberOfShapes = 100;
	};

} // namespace Demo