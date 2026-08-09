#pragma once

#include "Sprite.h"
#include "Texture2D.h"
#include "Camera2D.h"

#include <vector>

namespace Demo
{

	class KunaiProjectiles
	{
	public:
		bool init();
		void update(float dt, const Pekan::Renderer2D::Camera2D_ConstPtr& camera);
		void spawn(glm::vec2 playerPos, glm::vec2 playerSize, bool facingRight);
		void render() const;
		void destroy();

	private:
		struct Kunai
		{
			glm::vec2 position = { 0.0f, 0.0f };
			float velocityX = 0.0f;
		};

		Pekan::Graphics::Texture2D_Ptr m_texture;
		mutable Pekan::Renderer2D::Sprite m_sprite;
		std::vector<Kunai> m_kunais;
		float m_kunaiWidth = 0.45f;
		float m_kunaiHeight = 0.18f;
	};

} // namespace Demo
