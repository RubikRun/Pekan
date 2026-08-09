#pragma once

#include "IEntityRenderer.h"
#include "Sprite.h"
#include "Texture2D.h"

#include <vector>

namespace Demo
{

	// Draws entity textures directly via Renderer2D sprites.
	class DirectSpriteRenderer : public IEntityRenderer
	{
	public:
		bool init();
		void render(const EntityVisualState& state) const override;
		void destroy() override;

	private:
		struct AnimData
		{
			std::vector<Pekan::Graphics::Texture2D_Ptr> textures;
			std::vector<float> bottomPadFractions; // empty rows under feet / height
			float aspect = 1.0f;
		};

		const AnimData& getAnimData(EntityAnim anim) const;
		int clampFrame(const AnimData& anim, int frame) const;

		AnimData m_idle;
		AnimData m_run;
		AnimData m_jump;
		AnimData m_attack;

		mutable Pekan::Renderer2D::Sprite m_sprite;
	};

} // namespace Demo
