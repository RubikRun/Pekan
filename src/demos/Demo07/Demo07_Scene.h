#pragma once

#include "Layer.h"

#include "Demo07_GUIWindow.h"

namespace Demo
{

	class Demo07_Scene : public Pekan::Layer
	{
	public:

		Demo07_Scene(Pekan::PekanApplication* application) : Layer("scene_layer", application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() override;

		void exit() override;
		
		void attachGUIWindow(const Demo07_GUIWindow* guiWindow) { m_guiWindow = guiWindow; }

	private: /* variables */

		const Demo07_GUIWindow* m_guiWindow = nullptr;
	};

} // namespace Demo