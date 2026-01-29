#pragma once

#include "Scene2D.h"
#include <vector>

namespace Demo
{

	class Demo07_GUIWindow;

	class Demo07_Scene : public Pekan::Renderer2D::Scene2D
	{

	public:

		Demo07_Scene(Pekan::PekanApplication* application) : Scene2D(application) {}
		
		void attachGUIWindow(const std::shared_ptr<const Demo07_GUIWindow>& guiWindow) { m_guiWindow = guiWindow; }

	private: /* functions */

		bool _init() override;
		void _exit() override;

		void update(double deltaTime) override;

		void createCamera();
		void createCoordSys();
		void createShapes();

	private: /* variables */

		std::shared_ptr<const Demo07_GUIWindow> m_guiWindow;

		entt::entity m_camera = entt::null;
		std::vector<entt::entity> m_coordSys;

		entt::entity m_rectangle = entt::null;
		entt::entity m_circle = entt::null;
		entt::entity m_triangle = entt::null;
		entt::entity m_polygon = entt::null;
		entt::entity m_line = entt::null;

		// Cached ECS registry reference
		entt::registry& m_registry = getRegistry();
	};

} // namespace Demo