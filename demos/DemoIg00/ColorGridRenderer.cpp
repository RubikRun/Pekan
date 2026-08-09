#include "ColorGridRenderer.h"

#include "Image.h"
#include "PekanLogger.h"
#include "Utils/FileUtils.h"
#include "Utils/PekanUtils.h"
#include "Renderer2DSystem.h"
#include "Camera2D.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>

using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;
using namespace Pekan::Utils;

namespace Demo
{

	static constexpr int FRAMES_PER_ANIM = 10;
	static constexpr unsigned char ALPHA_THRESHOLD = 16;
	static constexpr float EXTRA_FOOT_OFFSET_WORLD = 0.11f;

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

	bool ColorGridRenderer::loadAnim(const char* prefix, AnimData& anim)
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

			FrameData& frame = anim.frames[i];
			frame.width = image.getWidth();
			frame.height = image.getHeight();
			frame.channels = image.getNumChannels();
			const size_t byteCount = static_cast<size_t>(frame.width) * frame.height * frame.channels;
			frame.pixels.resize(byteCount);
			std::memcpy(frame.pixels.data(), image.getData(), byteCount);
			frame.bottomPadFraction = computeBottomPadFraction(frame.pixels.data(), frame.width, frame.height, frame.channels);
			frame.texture = std::make_shared<Texture2D>();
			frame.texture->create(image);

			if (i == 0 && frame.height > 0)
			{
				anim.aspect = static_cast<float>(frame.width) / static_cast<float>(frame.height);
			}
		}
		return true;
	}

	bool ColorGridRenderer::init()
	{
		if (!loadAnim("Idle", m_idle)) return false;
		if (!loadAnim("Run", m_run)) return false;
		if (!loadAnim("Jump", m_jump)) return false;

		const std::string vs = Pekan::FileUtils::readTextFileToString("resources/color_grid_vs.glsl");
		const std::string fs = Pekan::FileUtils::readTextFileToString("resources/color_grid_fs.glsl");

		m_renderObject.create(
			{
				{ ShaderDataType::Float2, "aPosition" },
				{ ShaderDataType::Float2, "aTexCoord" },
				{ ShaderDataType::Float4, "aTint" }
			},
			vs.c_str(),
			fs.c_str()
		);

		std::vector<unsigned> indices(CELL_COUNT * 6);
		for (int i = 0; i < CELL_COUNT; i++)
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

		m_vertexScratch.resize(CELL_COUNT * 4 * 8);
		m_renderObject.setVertexData(
			m_vertexScratch.data(),
			static_cast<long long>(m_vertexScratch.size() * sizeof(float)),
			BufferDataUsage::DynamicDraw
		);

		Shader& shader = m_renderObject.getShader();
		shader.bind();
		shader.setUniform1i("uTexture", 0);
		shader.unbind();

		initCellJitter();
		return true;
	}

	void ColorGridRenderer::initCellJitter()
	{
		constexpr float degToRad = 3.14159265f / 180.0f;
		for (CellJitter& cell : m_cellJitter)
		{
			cell.posOffset = {
				getRandomFloat(-0.35f, 0.35f),
				getRandomFloat(-0.35f, 0.35f)
			};
			cell.sizeScale = getRandomFloat(0.88f, 1.50f);
			cell.rotation = getRandomFloat(-6.0f, 6.0f) * degToRad;
		}
	}

	const ColorGridRenderer::AnimData& ColorGridRenderer::getAnimData(EntityAnim anim) const
	{
		switch (anim)
		{
		case EntityAnim::Idle: return m_idle;
		case EntityAnim::Run:  return m_run;
		case EntityAnim::Jump: return m_jump;
		}
		return m_idle;
	}

	int ColorGridRenderer::clampFrame(const AnimData& anim, int frame) const
	{
		if (frame < 0) return 0;
		if (frame >= static_cast<int>(anim.frames.size())) return static_cast<int>(anim.frames.size()) - 1;
		return frame;
	}

	ColorGridRenderer::SpriteBasis ColorGridRenderer::getSpriteBasis(const EntityVisualState& state) const
	{
		const AnimData& anim = getAnimData(state.anim);
		const FrameData& frame = anim.frames[clampFrame(anim, state.frame)];

		SpriteBasis basis;
		basis.height = state.size.y;
		basis.width = basis.height * anim.aspect;
		basis.facing = state.facingRight ? 1.0f : -1.0f;
		basis.center = state.position;
		basis.center.y -= frame.bottomPadFraction * basis.height;
		basis.center.y -= EXTRA_FOOT_OFFSET_WORLD;
		return basis;
	}

	bool ColorGridRenderer::sampleColor(const FrameData& frame, glm::vec2 uv, glm::vec4& outColor) const
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
		outColor = { px[0] / 255.0f, px[1] / 255.0f, px[2] / 255.0f, px[3] / 255.0f };
		return px[3] > ALPHA_THRESHOLD;
	}

	void ColorGridRenderer::uploadVertices(const EntityVisualState& state, const FrameData& frame) const
	{
		const SpriteBasis basis = getSpriteBasis(state);
		const float opacity = std::clamp(state.opacity, 0.0f, 1.0f);
		const float cellW = basis.width / static_cast<float>(GRID_COLS);
		const float cellH = basis.height / static_cast<float>(GRID_ROWS);

		static const glm::vec2 corners[4] = {
			{ -0.5f, -0.5f },
			{  0.5f, -0.5f },
			{  0.5f,  0.5f },
			{ -0.5f,  0.5f }
		};
		static const glm::vec2 uvs[4] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		float* v = m_vertexScratch.data();
		int cellIndex = 0;
		for (int row = 0; row < GRID_ROWS; row++)
		{
			for (int col = 0; col < GRID_COLS; col++)
			{
				const CellJitter& jitter = m_cellJitter[cellIndex++];

				const glm::vec2 sampleUV = {
					std::clamp((static_cast<float>(col) + 0.5f + jitter.posOffset.x) / static_cast<float>(GRID_COLS), 0.0f, 1.0f),
					std::clamp((static_cast<float>(row) + 0.5f + jitter.posOffset.y) / static_cast<float>(GRID_ROWS), 0.0f, 1.0f)
				};

				glm::vec4 tint = { 1.0f, 1.0f, 1.0f, 0.0f };
				sampleColor(frame, sampleUV, tint);
				tint.a *= opacity;

				const float centerX =
					(static_cast<float>(col) + 0.5f + jitter.posOffset.x) * cellW - basis.width * 0.5f;
				const float centerY =
					(static_cast<float>(row) + 0.5f + jitter.posOffset.y) * cellH - basis.height * 0.5f;
				const float w = cellW * jitter.sizeScale;
				const float h = cellH * jitter.sizeScale;
				const float ca = std::cos(jitter.rotation);
				const float sa = std::sin(jitter.rotation);

				for (int c = 0; c < 4; c++)
				{
					const float rx = corners[c].x * w;
					const float ry = corners[c].y * h;
					const float lx = centerX + ca * rx - sa * ry;
					const float ly = centerY + sa * rx + ca * ry;
					*v++ = basis.center.x + basis.facing * lx;
					*v++ = basis.center.y + ly;
					*v++ = uvs[c].x;
					*v++ = uvs[c].y;
					*v++ = tint.r;
					*v++ = tint.g;
					*v++ = tint.b;
					*v++ = tint.a;
				}
			}
		}

		m_renderObject.setVertexData(
			m_vertexScratch.data(),
			static_cast<long long>(m_vertexScratch.size() * sizeof(float))
		);
	}

	void ColorGridRenderer::render(const EntityVisualState& state) const
	{
		if (!m_renderObject.isValid())
		{
			return;
		}

		const AnimData& anim = getAnimData(state.anim);
		const FrameData& frame = anim.frames[clampFrame(anim, state.frame)];
		if (frame.texture == nullptr || !frame.texture->isValid())
		{
			return;
		}

		uploadVertices(state, frame);

		Camera2D_ConstPtr camera = Renderer2DSystem::getCamera();
		Shader& shader = m_renderObject.getShader();
		shader.bind();
		shader.setUniform1i("uTexture", 0);
		if (camera != nullptr)
		{
			shader.setUniformMatrix4fv("uViewProjectionMatrix", camera->getViewProjectionMatrix());
		}
		frame.texture->bind(0);
		shader.unbind();

		m_renderObject.render();
	}

	void ColorGridRenderer::destroy()
	{
		if (m_renderObject.isValid())
		{
			m_renderObject.destroy();
		}

		auto destroyAnim = [](AnimData& anim)
		{
			for (auto& frame : anim.frames)
			{
				if (frame.texture)
				{
					frame.texture->destroy();
				}
			}
			anim.frames.clear();
		};
		destroyAnim(m_idle);
		destroyAnim(m_run);
		destroyAnim(m_jump);
	}

} // namespace Demo
