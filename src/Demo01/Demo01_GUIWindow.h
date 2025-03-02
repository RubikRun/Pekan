#pragma once

#include "PekanGUIWindow.h"
using Pekan::PekanGUIWindow;

#include "imgui.h"

namespace Demo
{

	class Demo01_Scene;

	class Demo01_GUIWindow : public PekanGUIWindow
	{
	public:

		inline void attachScene(Demo01_Scene* scene) { m_scene = scene; }

	private: /* functions */

		void _render() override;

		bool _init() override;

	private: /* variables */

		Demo01_Scene* m_scene = nullptr;
	};

} // namespace Demo