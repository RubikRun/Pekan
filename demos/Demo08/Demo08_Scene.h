#pragma once

#include "Layer.h"
#include "RenderObject.h"
#include "RectangleShape.h"
#include "Camera2D.h"
#include "Sprite.h"

#include "Demo08_GUIWindow.h"

#include <vector>

namespace Demo
{

	class Demo08_Scene : public Pekan::Layer
	{

	public:

		Demo08_Scene(Pekan::PekanApplication* application) : Layer(application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() const override;

		void exit() override;
		
		void attachGUIWindow(const std::shared_ptr<Demo08_GUIWindow>& guiWindow) { m_guiWindow = guiWindow; }

		inline std::string getLayerName() const override { return "scene_layer"; }

	private: /* functions */

		void createCamera();
		void createSprites();
		void createCenterSquare();

		void updateSprites(float dt);

	private: /* variables */

		std::vector<Pekan::Renderer2D::Sprite> m_sprites;
		std::vector<glm::vec2> m_spritesVelocities;

		// A square in the center of the bounding box, just to make sure that coordinates are resolution-independent
		Pekan::Renderer2D::RectangleShape m_centerSquare;

		Pekan::Renderer2D::Camera2D_Ptr m_camera;

		int m_spritesCount = -1;
		int m_spritesMaxCount = -1;

		std::shared_ptr<Demo08_GUIWindow> m_guiWindow;

		float t = 0.0f;
	};

} // namespace Demo