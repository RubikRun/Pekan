#pragma once

#include "Layer.h"
#include "RenderObject.h"

#include "Demo06_GUIWindow.h"

namespace Demo
{

	class Demo06_Scene : public Pekan::Layer
	{
	public:

		Demo06_Scene(Pekan::PekanApplication* application) : Layer("scene_layer", application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() override;

		void exit() override;
		
		// Attaches a GUI window for controlling background's color
		void attachGUIWindow(const Demo06_GUIWindow* guiWindow) { m_guiWindow = guiWindow; }

	private: /* variables */

		Pekan::Renderer::RenderObject m_renderObject;

		// GUI window for controlling background's color
		const Demo06_GUIWindow* m_guiWindow = nullptr;
	};

} // namespace Demo