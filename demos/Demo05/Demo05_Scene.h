#pragma once

#include "Scene2D.h"

#define MTT0_SUBDIVS 200
#define MTT1_SUBDIVS 400

namespace Demo
{

	class Demo05_GUIWindow;

	class Demo05_Scene : public Pekan::Renderer2D::Scene2D
	{
	public:

		Demo05_Scene(Pekan::PekanApplication* application) : Scene2D(application) {}
		
		// Attaches a GUI window for controlling background's color
		void attachGUIWindow(const std::shared_ptr<const Demo05_GUIWindow>& guiWindow) { m_guiWindow = guiWindow; }

	private: /* functions */

		bool _init() override;
		void _exit() override;
		void update(double deltaTime) override;

		void updateMtt();

	private: /* variables */

		// GUI window for controlling background's color
		std::shared_ptr<const Demo05_GUIWindow> m_guiWindow;

		entt::entity m_lines[MTT0_SUBDIVS];

		entt::entity m_lineShapes[MTT1_SUBDIVS];

		entt::entity m_camera;

		// Multiplication factor of the 2 Mandelbrot Times Tables
		float m_mtt0factor = 30.0f;
		float m_mtt1factor = 90.0f;

		float t = 0.0f;

		// Cached ECS registry reference
		entt::registry& m_registry = getRegistry();
	};

} // namespace Demo