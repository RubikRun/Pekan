#pragma once

#include "Scene2D.h"
#include "Camera2D.h"

namespace Demo
{

	class Demo09_Scene : public Pekan::Renderer2D::Scene2D
	{
	public:

		Demo09_Scene(Pekan::PekanApplication* application) : Pekan::Renderer2D::Scene2D(application) {}

		bool init() override;

		void update(double deltaTime) override;

		void exit() override;

	private: /* functions */

		void createTurkey();
		void createBull();
		void createGround();
		void createCamera();

	private: /* variables */

		entt::entity m_turkey = entt::null;
		entt::entity m_bull = entt::null;
		entt::entity m_ground = entt::null;

		Pekan::Renderer2D::Camera2D_Ptr m_camera;

		float t = 0.0f;
	};

} // namespace Demo
