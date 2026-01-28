#pragma once

#include "Scene2D.h"
#include "Texture2D.h"

#include <vector>

namespace Demo
{

	class Demo08_GUIWindow;

	class Demo08_Scene : public Pekan::Renderer2D::Scene2D
	{

	public:

		Demo08_Scene(Pekan::PekanApplication* application) : Pekan::Renderer2D::Scene2D(application) {}

		void attachGUIWindow(const std::shared_ptr<Demo08_GUIWindow>& guiWindow) { m_guiWindow = guiWindow; }

	private: /* functions */

		bool _init() override;
		void _exit() override;

		void update(double deltaTime) override;

		void createCamera();
		void createSprites();
		void createCenterSquare();
		void createAnimSprite();

		void updateSprites(float dt);
		void updateAnimSprite(float dt);

	private: /* variables */

		std::vector<entt::entity> m_sprites;
		std::vector<glm::vec2> m_spritesVelocities;

		entt::entity m_centerSquare = entt::null;

		entt::entity m_animSprite = entt::null;
		std::vector<Pekan::Graphics::Texture2D_Ptr> m_animTextures;
		int m_currAnimTextureIndex = 0;

		entt::entity m_camera = entt::null;

		int m_spritesCount = -1;
		int m_spritesMaxCount = -1;

		std::shared_ptr<Demo08_GUIWindow> m_guiWindow;

		float t = 0.0f;

		// Time until the next update of the animation sprite
		float m_timeTilAnimUpdate = 0.0f;

		// Cached window size
		glm::vec2 m_windowSize = { 0.0f, 0.0f };

		// Cached ECS registry reference
		entt::registry& m_registry = getRegistry();
	};

} // namespace Demo