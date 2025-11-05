#pragma once

#include "Scene2D.h"
#include "Camera2D.h"

namespace Demo
{

	class Demo09_Scene : public Pekan::Renderer2D::Scene2D
	{
	public:

		Demo09_Scene(Pekan::PekanApplication* application) : Pekan::Renderer2D::Scene2D(application) {}

	private: /* functions */

		bool _init() override;
		void _exit() override;

		void update(double deltaTime) override;

		void createTurkey();
		void createBull();
		void createRectangle();
		void createTriangle();
		void createPolygon1();
		void createPolygon2();
		void createCircle();
		void createLine();
		void createCamera();

	private: /* variables */

		entt::entity m_turkey = entt::null;
		entt::entity m_bull = entt::null;
		entt::entity m_rectangle = entt::null;
		entt::entity m_triangle = entt::null;
		entt::entity m_polygon1 = entt::null;
		entt::entity m_polygon2 = entt::null;
		entt::entity m_circle = entt::null;
		entt::entity m_line = entt::null;

		entt::entity m_camera = entt::null;

		float t = 0.0f;
	};

} // namespace Demo
