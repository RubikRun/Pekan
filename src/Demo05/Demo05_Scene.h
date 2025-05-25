#pragma once

#include "Layer.h"
#include "2D/Line.h"

#include "Demo05_GUIWindow.h"

namespace Demo
{

	class Demo05_Scene : public Pekan::Layer
	{
	public:

		Demo05_Scene(Pekan::PekanApplication* application) : Layer("scene_layer", application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() override;

		void exit() override;
		
		// Attaches a GUI window for controlling background's color
		void attachGUIWindow(const Demo05_GUIWindow* guiWindow) { m_guiWindow = guiWindow; }

	private: /* variables */

		Pekan::Renderer::Line m_line;

		// GUI window for controlling background's color
		const Demo05_GUIWindow* m_guiWindow = nullptr;
	};

} // namespace Demo