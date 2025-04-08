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

		inline void attachScene(Demo01_Scene* scene) { m_scene = scene; }

	private: /* functions */

		void _render() override;

		bool init() override;

	private: /* variables */

		Demo01_Scene* m_scene = nullptr;

		glm::ivec2 m_resolution;
	};

} // namespace Demo