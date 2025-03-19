#pragma once

#include "IPekanGUIWindow.h"
using Pekan::IPekanGUIWindow;

#include "imgui.h"

namespace Demo
{

	class Demo01_Scene;

	class Demo01_GUIWindow : public IPekanGUIWindow
	{
	public:

		inline void attachScene(Demo01_Scene* scene) { m_scene = scene; }

	private: /* functions */

		void _render() override;

		bool init() override;

	private: /* variables */

		Demo01_Scene* m_scene = nullptr;
	};

} // namespace Demo