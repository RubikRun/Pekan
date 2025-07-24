#pragma once

#include "Layer.h"
#include "RenderObject.h"

#include "Demo00_GUIWindow.h"

namespace Demo
{

	class Demo00_Scene : public Pekan::Layer
	{
	public:

		Demo00_Scene(Pekan::PekanApplication* application) : Layer("scene_layer", application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() const override;

		void exit() override;
		
		// Attaches a GUI window for controlling background's color
		void attachGUIWindow(const std::shared_ptr<const Demo00_GUIWindow>& guiWindow) { m_guiWindow = guiWindow; }

	private: /* variables */

		Pekan::Renderer2D::RenderObject m_renderObject;

		// GUI window for controlling background's color
		std::shared_ptr<const Demo00_GUIWindow> m_guiWindow;
	};

} // namespace Demo