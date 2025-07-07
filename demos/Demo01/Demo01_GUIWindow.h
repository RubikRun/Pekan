#pragma once

#include "PekanGUIWindow.h"
using Pekan::PekanGUIWindow;

#include <glm/glm.hpp>

namespace Demo
{

	class Demo01_Scene;

	class Demo01_GUIWindow : public PekanGUIWindow
	{
	public:

		Demo01_GUIWindow(Pekan::PekanApplication* application) : PekanGUIWindow(application) {}

		inline void attachScene(const std::shared_ptr<Demo01_Scene>& scene) { m_scene = scene; }

		inline bool getMoveThirdSquare() const { return m_moveThirdSquare; }

	private: /* functions */

		void _render() override;

		bool init() override;

	private: /* variables */

		std::shared_ptr<Demo01_Scene> m_scene;

		glm::ivec2 m_resolution = glm::ivec2(0, 0);

		bool m_moveThirdSquare = false;
	};

} // namespace Demo