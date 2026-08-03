#pragma once

#include "Layer.h"
#include "Camera2D.h"
#include "RectangleShape.h"

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

		Pekan::Renderer2D::Camera2D_Ptr m_camera;

		// Placeholder visuals — replace with real player / level later
		Pekan::Renderer2D::RectangleShape m_ground;
		Pekan::Renderer2D::RectangleShape m_player;
	};

} // namespace Demo
