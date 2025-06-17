#pragma once

#include "PekanGUIWindow.h"
using Pekan::PekanGUIWindow;

#include "imgui.h"
#include <glm/glm.hpp>

namespace Demo
{

	class Demo07_GUIWindow : public PekanGUIWindow
	{
	public:

		Demo07_GUIWindow(Pekan::PekanApplication* application) : PekanGUIWindow(application) {}

		inline glm::vec2 getRectanglePosition() const { return m_rectanglePosition; }

	private: /* functions */

		void _render() override;

		bool init() override;

	private: /* variables */

		glm::vec2 m_rectanglePosition = glm::vec2(1.5f, 1.0f);
	};

} // namespace Demo