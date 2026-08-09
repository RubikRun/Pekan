#pragma once

#include "IEntityRenderer.h"
#include "RenderObject.h"
#include "Texture2D.h"

#include <vector>
#include <array>

namespace Demo
{

	// Grid-based mini copies of the player texture with per-cell random pose jitter.
	class ColorGridRenderer : public IEntityRenderer
	{
	public:
		bool init();
		void render(const EntityVisualState& state) const override;
		void destroy() override;

	private:
		static constexpr int GRID_COLS = 6;
		static constexpr int GRID_ROWS = 9;
		static constexpr int CELL_COUNT = GRID_COLS * GRID_ROWS;

		struct FrameData
		{
			Pekan::Graphics::Texture2D_Ptr texture;
			int width = 0;
			int height = 0;
			int channels = 0;
			std::vector<unsigned char> pixels;
			float bottomPadFraction = 0.0f;
		};

		struct AnimData
		{
			std::vector<FrameData> frames;
			float aspect = 1.0f;
		};

		struct SpriteBasis
		{
			glm::vec2 center = { 0.0f, 0.0f };
			float width = 1.0f;
			float height = 1.0f;
			float facing = 1.0f;
		};

		struct CellJitter
		{
			glm::vec2 posOffset = { 0.0f, 0.0f }; // in cell-size units
			float sizeScale = 1.0f;
			float rotation = 0.0f; // radians
		};

		bool loadAnim(const char* prefix, AnimData& anim);
		void initCellJitter();
		const AnimData& getAnimData(EntityAnim anim) const;
		int clampFrame(const AnimData& anim, int frame) const;
		SpriteBasis getSpriteBasis(const EntityVisualState& state) const;
		bool sampleColor(const FrameData& frame, glm::vec2 uv, glm::vec4& outColor) const;
		void uploadVertices(const EntityVisualState& state, const FrameData& frame) const;

		AnimData m_idle;
		AnimData m_run;
		AnimData m_jump;
		AnimData m_attack;
		std::array<CellJitter, CELL_COUNT> m_cellJitter{};

		mutable Pekan::Graphics::RenderObject m_renderObject;
		mutable std::vector<float> m_vertexScratch;
	};

} // namespace Demo
