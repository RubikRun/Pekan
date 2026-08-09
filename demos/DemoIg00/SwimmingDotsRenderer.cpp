#include "SwimmingDotsRenderer.h"

#include "Image.h"
#include "PekanLogger.h"
#include "PekanEngine.h"
#include "Utils/FileUtils.h"
#include "Utils/PekanUtils.h"
#include "Renderer2DSystem.h"

#include <cmath>
#include <algorithm>
#include <cstring>
#include <string>

using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;
using namespace Pekan::Utils;
using Pekan::PekanEngine;

namespace Demo
{

	static constexpr int FRAMES_PER_ANIM = 10;
	static constexpr unsigned char ALPHA_THRESHOLD = 16;
	static constexpr float EXTRA_FOOT_OFFSET_WORLD = 0.11f;
	static constexpr float MIN_RADIUS_PX = 2.0f;
	static constexpr float MAX_RADIUS_PX = 6.0f;
	static constexpr float MAX_SWIM_DIST_PX = 10.0f;
	static constexpr float MIN_LIFE = 1.5f;
	static constexpr float MAX_LIFE = 4.0f;

	static float pixelsToWorld()
	{
		const float windowHeight = static_cast<float>(PekanEngine::getWindow().getSize().y);
		float worldHeight = 7.0f;
		if (const Camera2D_ConstPtr camera = Renderer2DSystem::getCamera())
		{
			worldHeight = camera->getSize().y;
		}
		return worldHeight / std::max(windowHeight, 1.0f);
	}

	static float computeBottomPadFraction(const unsigned char* data, int w, int h, int channels)
	{
		if (data == nullptr || w <= 0 || h <= 0 || channels < 4)
		{
			return 0.0f;
		}
		// Image pixels are flipped vertically on load, so row 0 is the visual bottom.
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				if (data[(y * w + x) * channels + 3] > ALPHA_THRESHOLD)
				{
					return static_cast<float>(y) / static_cast<float>(h);
				}
			}
		}
		return 0.0f;
	}

	bool SwimmingDotsRenderer::loadAnim(const char* prefix, AnimData& anim)
	{
		anim.frames.clear();
		anim.frames.resize(FRAMES_PER_ANIM);
		anim.aspect = 1.0f;

		for (int i = 0; i < FRAMES_PER_ANIM; i++)
		{
			std::string path = "resources/NinjaSprites/";
			path += prefix;
			path += "__";
			if (i < 10) path += "00";
			else if (i < 100) path += "0";
			path += std::to_string(i);
			path += ".png";

			Image image(path.c_str());
			if (!image.isValid())
			{
				PK_LOG_ERROR(("Failed to load texture: " + path).c_str(), "DemoIg00");
				return false;
			}

			FramePixels& frame = anim.frames[i];
			frame.width = image.getWidth();
			frame.height = image.getHeight();
			frame.channels = image.getNumChannels();
			const size_t byteCount = static_cast<size_t>(frame.width) * frame.height * frame.channels;
			frame.pixels.resize(byteCount);
			std::memcpy(frame.pixels.data(), image.getData(), byteCount);
			frame.bottomPadFraction = computeBottomPadFraction(frame.pixels.data(), frame.width, frame.height, frame.channels);

			if (i == 0 && frame.height > 0)
			{
				anim.aspect = static_cast<float>(frame.width) / static_cast<float>(frame.height);
			}
		}
		return true;
	}

	bool SwimmingDotsRenderer::init()
	{
		if (!loadAnim("Idle", m_idle)) return false;
		if (!loadAnim("Run", m_run)) return false;
		if (!loadAnim("Jump", m_jump)) return false;

		const std::string vs = Pekan::FileUtils::readTextFileToString("resources/swimming_dots_vs.glsl");
		const std::string fs = Pekan::FileUtils::readTextFileToString("resources/swimming_dots_fs.glsl");

		m_renderObject.create(
			{
				{ ShaderDataType::Float2, "aCorner" },
				{ ShaderDataType::Float2, "aCenter" },
				{ ShaderDataType::Float, "aRadius" },
				{ ShaderDataType::Float4, "aColor" }
			},
			vs.c_str(),
			fs.c_str()
		);

		// 100 quads, 6 indices each
		std::vector<unsigned> indices(DOT_COUNT * 6);
		for (int i = 0; i < DOT_COUNT; i++)
		{
			const unsigned base = static_cast<unsigned>(i * 4);
			const int idx = i * 6;
			indices[idx + 0] = base + 0;
			indices[idx + 1] = base + 1;
			indices[idx + 2] = base + 2;
			indices[idx + 3] = base + 0;
			indices[idx + 4] = base + 2;
			indices[idx + 5] = base + 3;
		}
		m_renderObject.setIndexData(indices.data(), static_cast<long long>(indices.size() * sizeof(unsigned)), BufferDataUsage::StaticDraw);

		// floats per vertex: corner2 + center2 + radius1 + color4 = 9
		m_vertexScratch.resize(DOT_COUNT * 4 * 9);
		m_renderObject.setVertexData(m_vertexScratch.data(), static_cast<long long>(m_vertexScratch.size() * sizeof(float)), BufferDataUsage::DynamicDraw);

		return true;
	}

	const SwimmingDotsRenderer::AnimData& SwimmingDotsRenderer::getAnimData(EntityAnim anim) const
	{
		switch (anim)
		{
		case EntityAnim::Idle: return m_idle;
		case EntityAnim::Run:  return m_run;
		case EntityAnim::Jump: return m_jump;
		}
		return m_idle;
	}

	int SwimmingDotsRenderer::clampFrame(const AnimData& anim, int frame) const
	{
		if (frame < 0) return 0;
		if (frame >= static_cast<int>(anim.frames.size())) return static_cast<int>(anim.frames.size()) - 1;
		return frame;
	}

	SwimmingDotsRenderer::SpriteBasis SwimmingDotsRenderer::getSpriteBasis(const EntityVisualState& state) const
	{
		const AnimData& anim = getAnimData(state.anim);
		const int frameIdx = clampFrame(anim, state.frame);
		const FramePixels& frame = anim.frames[frameIdx];

		SpriteBasis basis;
		basis.height = state.size.y;
		basis.width = basis.height * anim.aspect;
		basis.facing = state.facingRight ? 1.0f : -1.0f;
		basis.center = state.position;
		basis.center.y -= frame.bottomPadFraction * basis.height;
		basis.center.y -= EXTRA_FOOT_OFFSET_WORLD;
		return basis;
	}

	bool SwimmingDotsRenderer::sampleColor(const FramePixels& frame, glm::vec2 uv, glm::vec4& outColor) const
	{
		if (frame.pixels.empty() || frame.width <= 0 || frame.height <= 0 || frame.channels < 4)
		{
			return false;
		}

		uv.x = std::clamp(uv.x, 0.0f, 1.0f);
		uv.y = std::clamp(uv.y, 0.0f, 1.0f);
		const int x = std::clamp(static_cast<int>(uv.x * (frame.width - 1)), 0, frame.width - 1);
		const int y = std::clamp(static_cast<int>(uv.y * (frame.height - 1)), 0, frame.height - 1);
		const unsigned char* px = &frame.pixels[(y * frame.width + x) * frame.channels];
		if (px[3] <= ALPHA_THRESHOLD)
		{
			return false;
		}

		outColor = {
			px[0] / 255.0f,
			px[1] / 255.0f,
			px[2] / 255.0f,
			px[3] / 255.0f
		};

		const float origChroma =
			std::max(outColor.r, std::max(outColor.g, outColor.b)) -
			std::min(outColor.r, std::min(outColor.g, outColor.b));

		// Mild crayon look: a bit more saturation + soft color snapping
		float luma = 0.2126f * outColor.r + 0.7152f * outColor.g + 0.0722f * outColor.b;
		constexpr float satBoost = 1.45f;
		outColor.r = std::clamp(luma + (outColor.r - luma) * satBoost, 0.0f, 1.0f);
		outColor.g = std::clamp(luma + (outColor.g - luma) * satBoost, 0.0f, 1.0f);
		outColor.b = std::clamp(luma + (outColor.b - luma) * satBoost, 0.0f, 1.0f);

		constexpr float steps = 6.0f;
		const float qr = std::floor(outColor.r * steps + 0.5f) / steps;
		const float qg = std::floor(outColor.g * steps + 0.5f) / steps;
		const float qb = std::floor(outColor.b * steps + 0.5f) / steps;
		constexpr float snap = 0.38f;
		outColor.r = outColor.r * (1.0f - snap) + qr * snap;
		outColor.g = outColor.g * (1.0f - snap) + qg * snap;
		outColor.b = outColor.b * (1.0f - snap) + qb * snap;

		// Dark near-gray dots only: mild random crayon tint (leave light/white alone)
		constexpr float grayThresh = 0.14f;
		constexpr float maxLumaForTint = 0.55f;
		if (origChroma < grayThresh && luma <= maxLumaForTint)
		{
			luma = 0.2126f * outColor.r + 0.7152f * outColor.g + 0.0722f * outColor.b;
			const float grayness = 1.0f - origChroma / grayThresh;
			const float darkness = 1.0f - luma / maxLumaForTint;
			const float strength = getRandomFloat(0.10f, 0.22f) * grayness * darkness;
			const float hue = getRandomFloat(0.0f, 6.2831853f);
			outColor.r = std::clamp(luma + strength * std::cos(hue), 0.0f, 1.0f);
			outColor.g = std::clamp(luma + strength * std::cos(hue + 2.0943951f), 0.0f, 1.0f);
			outColor.b = std::clamp(luma + strength * std::cos(hue + 4.1887902f), 0.0f, 1.0f);
		}

		// Snap near-black up to a dark crayon floor
		constexpr float blackFloor = 0.18f;
		outColor.r = std::max(outColor.r, blackFloor);
		outColor.g = std::max(outColor.g, blackFloor);
		outColor.b = std::max(outColor.b, blackFloor);
		return true;
	}

	bool SwimmingDotsRenderer::tryPickOpaqueUV(const FramePixels& frame, glm::vec2& outUV, glm::vec4& outColor) const
	{
		for (int attempt = 0; attempt < 40; attempt++)
		{
			const glm::vec2 uv = {
				getRandomFloat(0.05f, 0.95f),
				getRandomFloat(0.05f, 0.95f)
			};
			if (sampleColor(frame, uv, outColor))
			{
				outUV = uv;
				return true;
			}
		}
		return false;
	}

	void SwimmingDotsRenderer::respawnDot(Dot& dot, const EntityVisualState& state)
	{
		const AnimData& anim = getAnimData(state.anim);
		const FramePixels& frame = anim.frames[clampFrame(anim, state.frame)];

		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec2 uv = { 0.5f, 0.5f };
		if (!tryPickOpaqueUV(frame, uv, color))
		{
			// Fallback: center sample (may be transparent on some frames)
			uv = { 0.5f, 0.5f };
			sampleColor(frame, uv, color);
		}

		const float px = pixelsToWorld();
		dot.homeUV = uv;
		dot.offset = { 0.0f, 0.0f };
		dot.velocity = {
			getRandomFloat(-20.0f, 20.0f) * px,
			getRandomFloat(-20.0f, 20.0f) * px
		};
		dot.radius = getRandomFloat(MIN_RADIUS_PX, MAX_RADIUS_PX) * px;
		dot.color = color;
		dot.life = getRandomFloat(MIN_LIFE, MAX_LIFE);
	}

	void SwimmingDotsRenderer::update(float dt, const EntityVisualState& state)
	{
		const bool poseChanged =
			!m_dotsInitialized ||
			state.anim != m_lastAnim ||
			state.frame != m_lastFrame;

		if (poseChanged)
		{
			// Resample all dots from the current frame so the silhouette
			// tracks animation with at most ~1 frame of lag.
			for (Dot& dot : m_dots)
			{
				respawnDot(dot, state);
			}
			m_dotsInitialized = true;
			m_lastAnim = state.anim;
			m_lastFrame = state.frame;
		}

		const float px = pixelsToWorld();
		const float maxDist = MAX_SWIM_DIST_PX * px;
		const float maxDistSq = maxDist * maxDist;

		for (Dot& dot : m_dots)
		{
			dot.life -= dt;
			if (dot.life <= 0.0f)
			{
				respawnDot(dot, state);
				continue;
			}

			// Gentle random swimming around the home point (in world units)
			dot.velocity += glm::vec2(
				getRandomFloat(-80.0f, 80.0f) * px,
				getRandomFloat(-80.0f, 80.0f) * px
			) * dt;
			dot.velocity -= dot.offset * 8.0f * dt; // spring toward home
			dot.velocity *= std::exp(-3.0f * dt);   // damping
			dot.offset += dot.velocity * dt;

			const float distSq = glm::dot(dot.offset, dot.offset);
			if (distSq > maxDistSq && distSq > 0.0f)
			{
				dot.offset *= maxDist / std::sqrt(distSq);
				dot.velocity *= -0.4f;
			}
		}
	}

	void SwimmingDotsRenderer::uploadVertices(const EntityVisualState& state) const
	{
		const SpriteBasis basis = getSpriteBasis(state);
		static const glm::vec2 corners[4] = {
			{ -1.0f, -1.0f },
			{  1.0f, -1.0f },
			{  1.0f,  1.0f },
			{ -1.0f,  1.0f }
		};

		float* v = m_vertexScratch.data();
		const float opacity = std::clamp(state.opacity, 0.0f, 1.0f);
		for (int i = 0; i < DOT_COUNT; i++)
		{
			const Dot& dot = m_dots[i];
			const glm::vec2 local = {
				(dot.homeUV.x - 0.5f) * basis.width,
				(dot.homeUV.y - 0.5f) * basis.height
			};
			const glm::vec2 center = {
				basis.center.x + basis.facing * local.x + dot.offset.x,
				basis.center.y + local.y + dot.offset.y
			};

			for (int c = 0; c < 4; c++)
			{
				*v++ = corners[c].x;
				*v++ = corners[c].y;
				*v++ = center.x;
				*v++ = center.y;
				*v++ = dot.radius;
				*v++ = dot.color.r;
				*v++ = dot.color.g;
				*v++ = dot.color.b;
				*v++ = dot.color.a * opacity;
			}
		}

		m_renderObject.setVertexData(
			m_vertexScratch.data(),
			static_cast<long long>(m_vertexScratch.size() * sizeof(float))
		);
	}

	void SwimmingDotsRenderer::render(const EntityVisualState& state) const
	{
		if (!m_renderObject.isValid() || !m_dotsInitialized)
		{
			return;
		}

		uploadVertices(state);

		Camera2D_ConstPtr camera = Renderer2DSystem::getCamera();
		Shader& shader = m_renderObject.getShader();
		shader.bind();
		if (camera != nullptr)
		{
			shader.setUniformMatrix4fv("uViewProjectionMatrix", camera->getViewProjectionMatrix());
		}
		shader.unbind();

		m_renderObject.render();
	}

	void SwimmingDotsRenderer::destroy()
	{
		if (m_renderObject.isValid())
		{
			m_renderObject.destroy();
		}
		m_idle.frames.clear();
		m_run.frames.clear();
		m_jump.frames.clear();
		m_dotsInitialized = false;
		m_lastFrame = -1;
	}

} // namespace Demo
