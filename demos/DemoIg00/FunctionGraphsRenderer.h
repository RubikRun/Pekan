#pragma once

#include "IEntityRenderer.h"
#include "RenderObject.h"

#include <vector>
#include <array>

namespace Demo
{

	// Approximates the player sprite with fixed random function graphs (colored line segments).
	class FunctionGraphsRenderer : public IEntityRenderer
	{
	public:
		bool init();
		void update(float dt, const EntityVisualState& state) override;
		void render(const EntityVisualState& state) const override;
		void destroy() override;

	private:
		static constexpr int GRAPH_COUNT = 100;
		static constexpr int SEGMENTS_PER_GRAPH = 260;
		static constexpr int FUNC_TYPE_COUNT = 10;

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

		struct Graph
		{
			int funcType = 0;
			float a = 1.0f;
			float b = 1.0f;
			float c = 0.0f;
			float d = 0.0f;
			float angle = 0.0f;
			glm::vec2 originLocal = { 0.0f, 0.0f }; // in sprite-local units from center
			glm::vec2 wiggleOffset = { 0.0f, 0.0f };
			glm::vec2 wiggleVelocity = { 0.0f, 0.0f };
			float angleWiggle = 0.0f;
			float angleWiggleVelocity = 0.0f;
			float domain = 1.0f;
		};

		struct SpriteBasis
		{
			glm::vec2 center = { 0.0f, 0.0f };
			float width = 1.0f;
			float height = 1.0f;
			float facing = 1.0f;
		};

		bool loadAnim(const char* prefix, AnimData& anim);
		void initGraphs();
		float evalFunction(const Graph& graph, float x) const;

		const AnimData& getAnimData(EntityAnim anim) const;
		int clampFrame(const AnimData& anim, int frame) const;
		SpriteBasis getSpriteBasis(const EntityVisualState& state) const;
		bool sampleColor(const FramePixels& frame, glm::vec2 uv, glm::vec4& outColor) const;
		void uploadVertices(const EntityVisualState& state) const;

		AnimData m_idle;
		AnimData m_run;
		AnimData m_jump;
		AnimData m_attack;
		AnimData m_throw;

		std::array<Graph, GRAPH_COUNT> m_graphs{};

		mutable Pekan::Graphics::RenderObject m_renderObject;
		mutable std::vector<float> m_vertexScratch;
		mutable int m_vertexFloatCount = 0;
	};

} // namespace Demo
