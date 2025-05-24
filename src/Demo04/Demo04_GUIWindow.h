#pragma once

#include "PekanGUIWindow.h"
using Pekan::PekanGUIWindow;

#include "imgui.h"

namespace Demo
{

	class Demo04_GUIWindow : public PekanGUIWindow
	{
	public:

		Demo04_GUIWindow(Pekan::PekanApplication* application) : PekanGUIWindow(application) {}

		// Returns current value of clear color
		inline const ImVec4& getClearColor() const { return m_clearColor; }

		// Returns current value of position
		inline const ImVec2& getPosition() const { return m_position; }

		// Checks if shapes are currently enabled
		inline bool isEnabledShapes() const { return m_enabledShapes; }

		// Returns currently selected shader index
		inline int getShaderIdx() const { return m_shaderIdx; }

	private: /* functions */

		void _render() override;

		bool init() override;

	private: /* variables */

		// Color for clearing scene's background
		ImVec4 m_clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		// Position of the square, relative to window's center
		ImVec2 m_position = ImVec2(0.0f, 0.0f);

		// Flag indicating if shapes are enabled and should be rendered
		bool m_enabledShapes = false;

		// Shader index determining which shader will be used for rendering
		int m_shaderIdx = 0;
	};

} // namespace Demo