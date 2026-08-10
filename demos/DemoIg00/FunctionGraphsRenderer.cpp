#include "FunctionGraphsRenderer.h"

#include "Image.h"
#include "PekanLogger.h"
#include "PekanEngine.h"
#include "Utils/FileUtils.h"
#include "Utils/PekanUtils.h"
#include "Renderer2DSystem.h"
#include "Camera2D.h"

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
	static constexpr float MAX_WIGGLE_DIST = 0.06f;
	static constexpr float MAX_ANGLE_WIGGLE = 0.08f;
	// Between 1px GL lines and the previous 1.5px thick quads
	static constexpr float LINE_WIDTH_PX = 1.10f;

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

	bool FunctionGraphsRenderer::loadAnim(const char* prefix, AnimData& anim)
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

	void FunctionGraphsRenderer::initGraphs()
	{
		constexpr float refH = 2.0f;
		constexpr float PI = 3.14159265f;

		for (Graph& g : m_graphs)
		{
			g.funcType = getRandomInt(0, FUNC_TYPE_COUNT - 1);
			g.angle = getRandomFloat(0.0f, 2.0f * PI);
			g.originLocal = {
				getRandomFloat(-0.3f, 0.3f) * refH * 0.53f,
				getRandomFloat(-0.35f, 0.35f) * refH
			};
			// Slightly wider span so high-frequency waves still cross the silhouette
			g.domain = getRandomFloat(0.85f, 1.4f) * refH;
			g.wiggleOffset = { 0.0f, 0.0f };
			g.wiggleVelocity = { 0.0f, 0.0f };
			g.angleWiggle = 0.0f;
			g.angleWiggleVelocity = 0.0f;
			g.d = 0.0f;

			switch (g.funcType)
			{
			case 0: // a * sin(b * x) — many oscillations ("zoomed out")
			{
				const float oscillations = getRandomFloat(10.0f, 18.0f);
				g.a = getRandomFloat(0.12f, 0.38f) * refH;
				g.b = oscillations * 2.0f * PI / g.domain;
				g.c = 0.0f;
				break;
			}
			case 1: // a * cos(b * x)
			{
				const float oscillations = getRandomFloat(10.0f, 18.0f);
				g.a = getRandomFloat(0.12f, 0.38f) * refH;
				g.b = oscillations * 2.0f * PI / g.domain;
				g.c = 0.0f;
				break;
			}
			case 2: // a*x^2 + b*x + c — steeper bowls / hills
				g.a = getRandomFloat(-1.2f, 1.2f) / refH;
				g.b = getRandomFloat(-0.8f, 0.8f);
				g.c = getRandomFloat(-0.25f, 0.25f) * refH;
				break;
			case 3: // a * log(b * |x| + c) — sharper log bend
				g.c = getRandomFloat(0.04f, 0.15f) * refH;
				g.b = getRandomFloat(6.0f, 28.0f);
				g.a = getRandomFloat(0.25f, 0.7f) * refH * (getRandomInt(0, 1) == 0 ? 1.0f : -1.0f);
				break;
			case 4: // a * tan(b * x) — denser asymptotes
			{
				const float asymptotes = getRandomFloat(3.0f, 6.0f);
				g.a = getRandomFloat(0.08f, 0.22f) * refH;
				g.b = asymptotes * PI / g.domain;
				g.c = 0.0f;
				break;
			}
			case 5: // a * |x| — occasional V, keep rarer feel via steeper slope
				g.a = getRandomFloat(-1.2f, 1.2f);
				g.b = 0.0f;
				g.c = 0.0f;
				break;
			case 6: // a * exp(-b * x^2) — narrower bumps
				g.a = getRandomFloat(0.2f, 0.55f) * refH;
				g.b = getRandomFloat(6.0f, 28.0f) / (refH * refH);
				g.c = 0.0f;
				break;
			case 7: // a / (1 + b * x^2) — narrower peaks
				g.a = getRandomFloat(0.2f, 0.55f) * refH;
				g.b = getRandomFloat(8.0f, 40.0f) / (refH * refH);
				g.c = 0.0f;
				break;
			case 8: // a * sin(b * x) * cos(c * x) — fast beat pattern
			{
				const float osc1 = getRandomFloat(10.0f, 16.0f);
				const float osc2 = getRandomFloat(2.0f, 5.0f);
				g.a = getRandomFloat(0.12f, 0.36f) * refH;
				g.b = osc1 * 2.0f * PI / g.domain;
				g.c = osc2 * 2.0f * PI / g.domain;
				break;
			}
			case 9: // a * sin(b * x) + c * sin(d * x) — two fast harmonics
			{
				const float osc1 = getRandomFloat(8.0f, 14.0f);
				const float osc2 = getRandomFloat(14.0f, 22.0f);
				g.a = getRandomFloat(0.1f, 0.28f) * refH;
				g.b = osc1 * 2.0f * PI / g.domain;
				g.c = getRandomFloat(0.08f, 0.22f) * refH;
				g.d = osc2 * 2.0f * PI / g.domain;
				break;
			}
			default:
				g.a = 0.25f * refH;
				g.b = 12.0f * 2.0f * PI / g.domain;
				g.c = 0.0f;
				break;
			}
		}
	}

	float FunctionGraphsRenderer::evalFunction(const Graph& graph, float x) const
	{
		switch (graph.funcType)
		{
		case 0: return graph.a * std::sin(graph.b * x);
		case 1: return graph.a * std::cos(graph.b * x);
		case 2: return graph.a * x * x + graph.b * x + graph.c;
		case 3: return graph.a * std::log(graph.b * std::abs(x) + graph.c);
		case 4: return graph.a * std::tan(graph.b * x);
		case 5: return graph.a * std::abs(x);
		case 6: return graph.a * std::exp(-graph.b * x * x);
		case 7: return graph.a / (1.0f + graph.b * x * x);
		case 8: return graph.a * std::sin(graph.b * x) * std::cos(graph.c * x);
		case 9: return graph.a * std::sin(graph.b * x) + graph.c * std::sin(graph.d * x);
		default: return 0.0f;
		}
	}

	bool FunctionGraphsRenderer::init()
	{
		if (!loadAnim("Idle", m_idle)) return false;
		if (!loadAnim("Run", m_run)) return false;
		if (!loadAnim("Jump", m_jump)) return false;
		if (!loadAnim("Attack", m_attack)) return false;
		if (!loadAnim("Throw", m_throw)) return false;

		initGraphs();

		const std::string vs = Pekan::FileUtils::readTextFileToString("resources/function_graphs_vs.glsl");
		const std::string fs = Pekan::FileUtils::readTextFileToString("resources/function_graphs_fs.glsl");

		m_renderObject.create(
			{
				{ ShaderDataType::Float2, "aPosition" },
				{ ShaderDataType::Float4, "aColor" }
			},
			vs.c_str(),
			fs.c_str()
		);

		// pos2 + color4 = 6 floats/vertex, 6 verts/segment (two triangles for thick lines)
		m_vertexScratch.resize(GRAPH_COUNT * SEGMENTS_PER_GRAPH * 6 * 6);
		m_renderObject.setVertexData(
			m_vertexScratch.data(),
			static_cast<long long>(m_vertexScratch.size() * sizeof(float)),
			BufferDataUsage::DynamicDraw
		);

		return true;
	}

	const FunctionGraphsRenderer::AnimData& FunctionGraphsRenderer::getAnimData(EntityAnim anim) const
	{
		switch (anim)
		{
		case EntityAnim::Idle: return m_idle;
		case EntityAnim::Run:  return m_run;
		case EntityAnim::Jump: return m_jump;
		case EntityAnim::Attack: return m_attack;
		case EntityAnim::Throw: return m_throw;
		}
		return m_idle;
	}

	int FunctionGraphsRenderer::clampFrame(const AnimData& anim, int frame) const
	{
		if (frame < 0) return 0;
		if (frame >= static_cast<int>(anim.frames.size())) return static_cast<int>(anim.frames.size()) - 1;
		return frame;
	}

	FunctionGraphsRenderer::SpriteBasis FunctionGraphsRenderer::getSpriteBasis(const EntityVisualState& state) const
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

	bool FunctionGraphsRenderer::sampleColor(const FramePixels& frame, glm::vec2 uv, glm::vec4& outColor) const
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

		outColor = { px[0] / 255.0f, px[1] / 255.0f, px[2] / 255.0f, px[3] / 255.0f };
		return true;
	}

	void FunctionGraphsRenderer::update(float dt, const EntityVisualState& state)
	{
		(void)state;

		for (Graph& g : m_graphs)
		{
			g.wiggleVelocity += glm::vec2(
				getRandomFloat(-1.2f, 1.2f),
				getRandomFloat(-1.2f, 1.2f)
			) * dt;
			g.wiggleVelocity -= g.wiggleOffset * 10.0f * dt;
			g.wiggleVelocity *= std::exp(-4.0f * dt);
			g.wiggleOffset += g.wiggleVelocity * dt;

			const float distSq = glm::dot(g.wiggleOffset, g.wiggleOffset);
			const float maxDistSq = MAX_WIGGLE_DIST * MAX_WIGGLE_DIST;
			if (distSq > maxDistSq && distSq > 0.0f)
			{
				g.wiggleOffset *= MAX_WIGGLE_DIST / std::sqrt(distSq);
				g.wiggleVelocity *= -0.4f;
			}

			g.angleWiggleVelocity += getRandomFloat(-2.0f, 2.0f) * dt;
			g.angleWiggleVelocity -= g.angleWiggle * 10.0f * dt;
			g.angleWiggleVelocity *= std::exp(-4.0f * dt);
			g.angleWiggle += g.angleWiggleVelocity * dt;
			g.angleWiggle = std::clamp(g.angleWiggle, -MAX_ANGLE_WIGGLE, MAX_ANGLE_WIGGLE);
		}
	}

	void FunctionGraphsRenderer::uploadVertices(const EntityVisualState& state) const
	{
		const SpriteBasis basis = getSpriteBasis(state);
		const AnimData& anim = getAnimData(state.anim);
		const FramePixels& frame = anim.frames[clampFrame(anim, state.frame)];

		float* v = m_vertexScratch.data();
		int floatCount = 0;
		const float opacity = std::clamp(state.opacity, 0.0f, 1.0f);
		const float halfThickness = 0.5f * LINE_WIDTH_PX * pixelsToWorld();

		auto emitVertex = [&](glm::vec2 worldPos, const glm::vec4& color)
		{
			*v++ = worldPos.x;
			*v++ = worldPos.y;
			*v++ = color.r;
			*v++ = color.g;
			*v++ = color.b;
			*v++ = color.a * opacity;
			floatCount += 6;
		};

		for (const Graph& g : m_graphs)
		{
			const float angle = g.angle + g.angleWiggle;
			const float ca = std::cos(angle);
			const float sa = std::sin(angle);
			const glm::vec2 origin = g.originLocal + g.wiggleOffset;

			glm::vec2 prevLocal(0.0f);
			glm::vec4 prevColor(0.0f);
			float prevY = 0.0f;
			bool hasPrev = false;
			constexpr float maxAbsY = 1.2f; // break near tan asymptotes / explosions
			constexpr float maxJumpY = 0.55f;

			for (int s = 0; s <= SEGMENTS_PER_GRAPH; s++)
			{
				const float t = static_cast<float>(s) / static_cast<float>(SEGMENTS_PER_GRAPH);
				const float x = (t * 2.0f - 1.0f) * g.domain;
				const float y = evalFunction(g, x);

				if (!std::isfinite(y) || std::abs(y) > maxAbsY)
				{
					hasPrev = false;
					continue;
				}

				const glm::vec2 local = {
					origin.x + ca * x - sa * y,
					origin.y + sa * x + ca * y
				};

				const glm::vec2 uv = {
					local.x / basis.width + 0.5f,
					local.y / basis.height + 0.5f
				};

				glm::vec4 color;
				const bool opaque = (uv.x >= 0.0f && uv.x <= 1.0f && uv.y >= 0.0f && uv.y <= 1.0f)
					&& sampleColor(frame, uv, color);

				const bool continuous = hasPrev && (std::abs(y - prevY) <= maxJumpY);

				if (opaque && continuous)
				{
					const glm::vec2 world0 = {
						basis.center.x + basis.facing * prevLocal.x,
						basis.center.y + prevLocal.y
					};
					const glm::vec2 world1 = {
						basis.center.x + basis.facing * local.x,
						basis.center.y + local.y
					};

					glm::vec2 dir = world1 - world0;
					const float lenSq = glm::dot(dir, dir);
					if (lenSq > 1e-12f)
					{
						dir *= 1.0f / std::sqrt(lenSq);
						const glm::vec2 n = { -dir.y * halfThickness, dir.x * halfThickness };
						const glm::vec2 p0 = world0 + n;
						const glm::vec2 p1 = world0 - n;
						const glm::vec2 p2 = world1 - n;
						const glm::vec2 p3 = world1 + n;
						emitVertex(p0, prevColor);
						emitVertex(p1, prevColor);
						emitVertex(p2, color);
						emitVertex(p0, prevColor);
						emitVertex(p2, color);
						emitVertex(p3, color);
					}
				}

				if (opaque)
				{
					prevLocal = local;
					prevColor = color;
					prevY = y;
					hasPrev = true;
				}
				else
				{
					hasPrev = false;
				}
			}
		}

		m_vertexFloatCount = floatCount;
		if (floatCount > 0)
		{
			m_renderObject.setVertexData(
				m_vertexScratch.data(),
				static_cast<long long>(floatCount * sizeof(float))
			);
		}
	}

	void FunctionGraphsRenderer::render(const EntityVisualState& state) const
	{
		if (!m_renderObject.isValid())
		{
			return;
		}

		uploadVertices(state);
		if (m_vertexFloatCount <= 0)
		{
			return;
		}

		Camera2D_ConstPtr camera = Renderer2DSystem::getCamera();
		Shader& shader = m_renderObject.getShader();
		shader.bind();
		if (camera != nullptr)
		{
			shader.setUniformMatrix4fv("uViewProjectionMatrix", camera->getViewProjectionMatrix());
		}
		shader.unbind();

		m_renderObject.render(DrawMode::Triangles);
	}

	void FunctionGraphsRenderer::destroy()
	{
		if (m_renderObject.isValid())
		{
			m_renderObject.destroy();
		}
		m_idle.frames.clear();
		m_run.frames.clear();
		m_jump.frames.clear();
	}

} // namespace Demo
