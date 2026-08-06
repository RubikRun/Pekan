#pragma once

#include "Layer.h"
#include "Camera2D.h"
#include "RectangleShape.h"

#include "Player.h"
#include "IEntityRenderer.h"

#include <memory>

namespace Demo
{

	class DemoIg00_Scene : public Pekan::Layer
	{
	public:

		DemoIg00_Scene(Pekan::PekanApplication* application) : Layer(application) {}

		bool init() override;
		void update(double deltaTime) override;
		void render() const override;
		void exit() override;

		inline std::string getLayerName() const override { return "scene_layer"; }

	private:

		bool onKeyPressed(const Pekan::KeyPressedEvent& event) override;

		void createCamera();
		void updateCamera();
		void updatePortalTransition();

		Pekan::Renderer2D::Camera2D_Ptr m_camera;
		Pekan::Renderer2D::RectangleShape m_ground;
		Pekan::Renderer2D::RectangleShape m_portal;

		Player m_player;
		std::unique_ptr<IEntityRenderer> m_dotsRenderer;
		std::unique_ptr<IEntityRenderer> m_functionsRenderer;
		IEntityRenderer* m_activeRenderer = nullptr;

		float m_groundTopY = 0.0f;
		float m_portalX = 0.0f;
	};

} // namespace Demo
