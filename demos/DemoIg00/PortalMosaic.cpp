#include "PortalMosaic.h"

#include "RenderState.h"
#include "PekanEngine.h"
#include "PekanLogger.h"
#include "Utils/FileUtils.h"
#include "Renderer2DSystem.h"

#include <algorithm>
#include <cmath>
#include <string>

using namespace Pekan::Renderer2D;
using namespace Pekan::Graphics;

namespace Demo
{

	static const glm::vec4 PORTAL_BG_COLOR = { 0.15f, 0.12f, 0.28f, 0.92f };
	static const glm::vec4 DARK_RED = { 0.42f, 0.06f, 0.08f, 1.0f };
	static const glm::vec4 BRIGHT_RED = { 0.95f, 0.14f, 0.10f, 1.0f };
	static const glm::vec4 OLIVE = { 0.42f, 0.48f, 0.18f, 1.0f };
	static const glm::vec4 OLIVE_DARK = { 0.28f, 0.32f, 0.10f, 1.0f };
	static const glm::vec4 YELLOW_MIST = { 1.0f, 0.86f, 0.22f, 0.42f };
	static const glm::vec4 BLUE_DOT = { 0.18f, 0.38f, 0.88f, 1.0f };
	static const glm::vec4 TRIANGLE_FILL = { 0.38f, 0.05f, 0.07f, 0.45f };

	static glm::vec2 worldToWindow(const Camera2D& camera, glm::vec2 world)
	{
		const glm::vec2 windowSize = glm::vec2(Pekan::PekanEngine::getWindow().getSize());
		const glm::vec2 ndc = (world - camera.getPosition()) * camera.getZoom() / (camera.getSize() * 0.5f);
		return {
			(ndc.x + 1.0f) * 0.5f * windowSize.x,
			(1.0f - ndc.y) * 0.5f * windowSize.y
		};
	}

	static glm::vec4 withOpacity(glm::vec4 color, float opacity)
	{
		color.a *= opacity;
		return color;
	}

	float PortalMosaic::opacityAtY(float y) const
	{
		return std::clamp((y - m_bottomY) / m_squareSize, 0.0f, 1.0f);
	}

	void PortalMosaic::layoutSquare(SquareDecor& sq, float bottom, bool mirrored)
	{
		constexpr float PI = 3.14159265f;
		const float inset = std::max(m_borderThickness, m_diagonalThickness) * 0.55f;
		const float left = m_centerX - m_squareSize * 0.5f + inset;
		const float right = m_centerX + m_squareSize * 0.5f - inset;
		const float top = bottom + m_squareSize;
		const float squareBottom = bottom;

		const glm::vec2 tl = { left, top };
		const glm::vec2 tr = { right, top };
		const glm::vec2 br = { right, squareBottom };
		const glm::vec2 bl = { left, squareBottom };

		glm::vec2 bigPos;
		glm::vec2 smallPos;
		glm::vec2 diagA;
		glm::vec2 diagB;
		glm::vec2 yellowCorner;
		float yellowAngleStart = 0.0f;

		if (!mirrored)
		{
			bigPos = { left + m_bigR, top - m_bigR };
			smallPos = { right - m_smallR, squareBottom + m_smallR };
			diagA = tl;
			diagB = br;
			yellowCorner = tr;
			yellowAngleStart = PI;
		}
		else
		{
			bigPos = { left + m_bigR, squareBottom + m_bigR };
			smallPos = { right - m_smallR, top - m_smallR };
			diagA = bl;
			diagB = tr;
			yellowCorner = br;
			yellowAngleStart = 0.5f * PI;
		}

		const glm::vec2 leftHit = { left, bigPos.y };
		const glm::vec2 verticalHit = mirrored
			? glm::vec2{ bigPos.x, top }
			: glm::vec2{ bigPos.x, squareBottom };
		sq.cornerTriangle.setVertices(bigPos, leftHit, verticalHit);
		sq.cornerTriangle.setColor(withOpacity(TRIANGLE_FILL, opacityAtY((bigPos.y + leftHit.y + verticalHit.y) / 3.0f)));

		sq.oliveBigUnder.setPosition(bigPos);
		sq.oliveBigUnder.setColor(withOpacity(OLIVE_DARK, opacityAtY(bigPos.y)));
		sq.oliveBig.setPosition(bigPos);
		sq.oliveBig.setColor(withOpacity(OLIVE, opacityAtY(bigPos.y)));

		sq.oliveSmallUnder.setPosition(smallPos);
		sq.oliveSmallUnder.setColor(withOpacity(OLIVE_DARK, opacityAtY(smallPos.y)));
		sq.oliveSmall.setPosition(smallPos);
		sq.oliveSmall.setColor(withOpacity(OLIVE, opacityAtY(smallPos.y)));

		sq.diagonal.setPointA(diagA);
		sq.diagonal.setPointB(diagB);
		sq.diagonal.setColor(withOpacity(BRIGHT_RED, opacityAtY((diagA.y + diagB.y) * 0.5f)));

		std::vector<glm::vec2> quarter;
		quarter.push_back(yellowCorner);
		constexpr int arcSegs = 14;
		for (int s = 0; s <= arcSegs; s++)
		{
			const float t = static_cast<float>(s) / static_cast<float>(arcSegs);
			const float angle = yellowAngleStart + t * (0.5f * PI);
			quarter.push_back({
				yellowCorner.x + m_yellowR * std::cos(angle),
				yellowCorner.y + m_yellowR * std::sin(angle)
			});
		}
		sq.yellowQuarter.setVertices(quarter);
		sq.yellowQuarter.setColor(withOpacity(YELLOW_MIST, opacityAtY(yellowCorner.y)));

		for (int b = 0; b < BLUE_ALONG_DIAGONAL; b++)
		{
			const float t = (static_cast<float>(b) + 0.5f) / static_cast<float>(BLUE_ALONG_DIAGONAL);
			const glm::vec2 pos = diagA + (diagB - diagA) * t;
			sq.blueDots[static_cast<size_t>(b)].setPosition(pos);
			sq.blueDots[static_cast<size_t>(b)].setColor(withOpacity(BLUE_DOT, opacityAtY(pos.y)));
		}

		const glm::vec2 edges[4][2] = {
			{ tl, tr }, { tr, br }, { br, bl }, { bl, tl }
		};
		for (int e = 0; e < 4; e++)
		{
			sq.borders[static_cast<size_t>(e)].setPointA(edges[e][0]);
			sq.borders[static_cast<size_t>(e)].setPointB(edges[e][1]);
			const float midY = (edges[e][0].y + edges[e][1].y) * 0.5f;
			sq.borders[static_cast<size_t>(e)].setColor(withOpacity(DARK_RED, opacityAtY(midY)));
		}
	}

	void PortalMosaic::layoutAll()
	{
		const float o = std::fmod(m_scroll, m_squareSize);
		const int phase = static_cast<int>(std::floor(m_scroll / m_squareSize));
		for (int k = 0; k <= m_squareCount; k++)
		{
			const float bottom = m_bottomY + o + static_cast<float>(k - 1) * m_squareSize;
			const bool mirrored = ((phase + k) & 1) != 0;
			layoutSquare(m_squares[static_cast<size_t>(k)], bottom, mirrored);
		}
	}

	void PortalMosaic::create(float centerX, float bottomY, float squareSize, int squareCount)
	{
		m_centerX = centerX;
		m_bottomY = bottomY;
		m_squareSize = squareSize;
		m_squareCount = squareCount;
		m_height = squareSize * static_cast<float>(squareCount);
		m_centerY = bottomY + m_height * 0.5f;
		m_scroll = 0.0f;

		constexpr float SQRT2 = 1.41421356f;
		const float sumRadii = squareSize * (2.0f - SQRT2);
		m_bigR = sumRadii * 0.68f;
		m_smallR = sumRadii - m_bigR;
		m_yellowR = squareSize * 0.48f;
		m_blueR = squareSize * 0.038f;

		const std::string vs = Pekan::FileUtils::readTextFileToString("resources/function_graphs_vs.glsl");
		const std::string fs = Pekan::FileUtils::readTextFileToString("resources/function_graphs_fs.glsl");
		if (vs.empty() || fs.empty())
		{
			PK_LOG_ERROR("Failed to load portal background shaders.", "DemoIg00");
			return;
		}

		m_background.create(
			{
				{ ShaderDataType::Float2, "aPosition" },
				{ ShaderDataType::Float4, "aColor" }
			},
			vs.c_str(),
			fs.c_str()
		);

		const float left = centerX - squareSize * 0.5f;
		const float right = centerX + squareSize * 0.5f;
		const float fadeY = bottomY + squareSize;
		const float top = bottomY + m_height;
		const float pr = PORTAL_BG_COLOR.r;
		const float pg = PORTAL_BG_COLOR.g;
		const float pb = PORTAL_BG_COLOR.b;
		const float pa = PORTAL_BG_COLOR.a;
		// Invisible at foot, full portal color from ~1 square up to the top
		const float verts[] = {
			left,  bottomY, pr, pg, pb, 0.0f,
			right, bottomY, pr, pg, pb, 0.0f,
			right, fadeY,   pr, pg, pb, pa,
			left,  fadeY,   pr, pg, pb, pa,
			right, top,     pr, pg, pb, pa,
			left,  top,     pr, pg, pb, pa
		};
		const unsigned indices[] = {
			0, 1, 2, 0, 2, 3,
			3, 2, 4, 3, 4, 5
		};
		m_background.setVertexData(verts, sizeof(verts), BufferDataUsage::StaticDraw);
		m_background.setIndexData(indices, sizeof(indices), BufferDataUsage::StaticDraw);

		const int tileCount = squareCount + 1;
		m_squares.resize(static_cast<size_t>(tileCount));
		for (SquareDecor& sq : m_squares)
		{
			sq.cornerTriangle.create({ 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f });
			sq.oliveBigUnder.create(m_bigR * 1.08f);
			sq.oliveBigUnder.setSegmentsCount(40);
			sq.oliveBig.create(m_bigR);
			sq.oliveBig.setSegmentsCount(40);
			sq.oliveSmallUnder.create(m_smallR * 1.12f);
			sq.oliveSmallUnder.setSegmentsCount(32);
			sq.oliveSmall.create(m_smallR);
			sq.oliveSmall.setSegmentsCount(32);
			sq.diagonal.create({ 0.0f, 0.0f }, { 1.0f, 0.0f }, m_diagonalThickness);
			sq.yellowQuarter.create({ { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f } });
			for (int b = 0; b < BLUE_ALONG_DIAGONAL; b++)
			{
				sq.blueDots[static_cast<size_t>(b)].create(m_blueR);
				sq.blueDots[static_cast<size_t>(b)].setSegmentsCount(16);
			}
			sq.borders.resize(4);
			for (int e = 0; e < 4; e++)
			{
				sq.borders[static_cast<size_t>(e)].create({ 0.0f, 0.0f }, { 1.0f, 0.0f }, m_borderThickness);
			}
		}

		layoutAll();
	}

	void PortalMosaic::update(float dt)
	{
		m_scroll += SCROLL_SPEED * dt;
		layoutAll();
	}

	void PortalMosaic::enablePortalScissor() const
	{
		Camera2D_ConstPtr camera = Renderer2DSystem::getCamera();
		if (camera == nullptr)
		{
			return;
		}

		const float left = m_centerX - m_squareSize * 0.5f;
		const float right = m_centerX + m_squareSize * 0.5f;
		const float bottom = m_bottomY;
		const float top = m_bottomY + m_height;

		const glm::vec2 winA = worldToWindow(*camera, { left, top });
		const glm::vec2 winB = worldToWindow(*camera, { right, bottom });
		const glm::vec2 windowSize = glm::vec2(Pekan::PekanEngine::getWindow().getSize());

		const float x = std::min(winA.x, winB.x);
		const float w = std::abs(winB.x - winA.x);
		const float yTop = std::min(winA.y, winB.y);
		const float yBot = std::max(winA.y, winB.y);
		const float glY = windowSize.y - yBot;
		const float h = yBot - yTop;

		RenderState::enableScissorTest();
		RenderState::setScissorBox(
			static_cast<int>(std::floor(x)),
			static_cast<int>(std::floor(glY)),
			std::max(0, static_cast<int>(std::ceil(w))),
			std::max(0, static_cast<int>(std::ceil(h)))
		);
	}

	void PortalMosaic::disablePortalScissor() const
	{
		RenderState::disableScissorTest();
	}

	void PortalMosaic::renderBackground() const
	{
		if (!m_background.isValid())
		{
			return;
		}

		Camera2D_ConstPtr camera = Renderer2DSystem::getCamera();
		Shader& shader = m_background.getShader();
		shader.bind();
		if (camera != nullptr)
		{
			shader.setUniformMatrix4fv("uViewProjectionMatrix", camera->getViewProjectionMatrix());
		}
		shader.unbind();
		m_background.render();
	}

	void PortalMosaic::renderMosaicShapes() const
	{
		for (const SquareDecor& sq : m_squares)
		{
			sq.oliveBigUnder.render();
			sq.oliveBig.render();
			sq.cornerTriangle.render();
			sq.oliveSmallUnder.render();
			sq.oliveSmall.render();
			sq.diagonal.render();
			sq.yellowQuarter.render();
			for (const CircleShape& dot : sq.blueDots)
			{
				dot.render();
			}
			for (const LineShape& border : sq.borders)
			{
				border.render();
			}
		}
	}

	void PortalMosaic::render() const
	{
		enablePortalScissor();
		renderBackground();
		Renderer2DSystem::beginFrame();
		renderMosaicShapes();
		Renderer2DSystem::endFrame();
		disablePortalScissor();
	}

	void PortalMosaic::destroy()
	{
		if (m_background.isValid())
		{
			m_background.destroy();
		}
		for (SquareDecor& sq : m_squares)
		{
			sq.cornerTriangle.destroy();
			sq.oliveBigUnder.destroy();
			sq.oliveBig.destroy();
			sq.oliveSmallUnder.destroy();
			sq.oliveSmall.destroy();
			sq.diagonal.destroy();
			sq.yellowQuarter.destroy();
			for (CircleShape& dot : sq.blueDots)
			{
				dot.destroy();
			}
			for (LineShape& border : sq.borders)
			{
				border.destroy();
			}
			sq.borders.clear();
		}
		m_squares.clear();
	}

} // namespace Demo
