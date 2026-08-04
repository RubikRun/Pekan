#pragma once

#include "IEntityRenderer.h"
#include "RenderObject.h"

#include <vector>
#include <array>

namespace Demo
{

	// Approximates the player sprite with swimming colored dots (one GPU draw).
	class SwimmingDotsRenderer : public IEntityRenderer
	{
	public:
		bool init();
		void update(float dt, const EntityVisualState& state) override;
		void render(const EntityVisualState& state) const override;
		void destroy() override;

	private:
		static constexpr int DOT_COUNT = 1000;

		struct FramePixels
		{
			int width = 0;
			int height = 0;
			int channels = 0;
			std::vector<unsigned char> pixels;
			float bottomPadFraction = 0.0f;
		};

		struct AnimData
		{
			std::vector<FramePixels> frames;
			float aspect = 1.0f;
		};

		struct Dot
		{
			glm::vec2 homeUV = { 0.5f, 0.5f };
			glm::vec2 offset = { 0.0f, 0.0f };
			glm::vec2 velocity = { 0.0f, 0.0f };
			float radius = 0.05f;
			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
			float life = 1.0f;
		};

		struct SpriteBasis
		{
			glm::vec2 center = { 0.0f, 0.0f };
			float width = 1.0f;
			float height = 1.0f;
			float facing = 1.0f;
		};

		bool loadAnim(const char* prefix, AnimData& anim);
		const AnimData& getAnimData(EntityAnim anim) const;
		int clampFrame(const AnimData& anim, int frame) const;
		SpriteBasis getSpriteBasis(const EntityVisualState& state) const;

		bool sampleColor(const FramePixels& frame, glm::vec2 uv, glm::vec4& outColor) const;
		bool tryPickOpaqueUV(const FramePixels& frame, glm::vec2& outUV, glm::vec4& outColor) const;
		void respawnDot(Dot& dot, const EntityVisualState& state);
		void uploadVertices(const EntityVisualState& state) const;

		AnimData m_idle;
		AnimData m_run;
		AnimData m_jump;

		std::array<Dot, DOT_COUNT> m_dots{};
		bool m_dotsInitialized = false;
		EntityAnim m_lastAnim = EntityAnim::Idle;
		int m_lastFrame = -1;

		mutable Pekan::Graphics::RenderObject m_renderObject;
		mutable std::vector<float> m_vertexScratch;
	};

} // namespace Demo
