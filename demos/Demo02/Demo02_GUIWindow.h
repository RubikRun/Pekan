#pragma once

#include "PekanGUIWindow.h"
using Pekan::PekanGUIWindow;

#include <glm/glm.hpp>

namespace Demo
{
	class Demo02_Scene;

	class Demo02_GUIWindow : public PekanGUIWindow
	{
	public:

		Demo02_GUIWindow(Pekan::PekanApplication* application) : PekanGUIWindow(application) {}

		inline void attachScene(std::shared_ptr<Demo02_Scene>& scene) { m_scene = scene; }

	private: /* functions */

		void _render() override;

		bool init() override;

	private: /* variables */

		std::shared_ptr<Demo02_Scene> m_scene;

		glm::ivec2 m_resolution;
	};

} // namespace Demo