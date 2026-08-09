#include "PortalMosaic.h"

#include <cmath>

using namespace Pekan::Renderer2D;

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

	void PortalMosaic::create(float centerX, float bottomY, float squareSize, int squareCount)
	{
		m_height = squareSize * static_cast<float>(squareCount);
		m_centerY = bottomY + m_height * 0.5f;

		m_background.create(squareSize, m_height);
		m_background.setPosition({ centerX, m_centerY });
		m_background.setColor(PORTAL_BG_COLOR);

		constexpr float PI = 3.14159265f;
		constexpr float SQRT2 = 1.41421356f;
		const float sumRadii = squareSize * (2.0f - SQRT2);
		const float bigR = sumRadii * 0.68f;
		const float smallR = sumRadii - bigR;
		const float yellowR = squareSize * 0.48f;
		const float blueR = squareSize * 0.038f;
		constexpr float borderThickness = 0.014f;
		constexpr float diagonalThickness = 0.022f;

		const float left = centerX - squareSize * 0.5f;
		const float right = centerX + squareSize * 0.5f;

		m_squares.resize(static_cast<size_t>(squareCount));
		for (int i = 0; i < squareCount; i++)
		{
			SquareDecor& sq = m_squares[static_cast<size_t>(i)];
			const float bottom = bottomY + static_cast<float>(i) * squareSize;
			const float top = bottom + squareSize;
			const bool mirrored = (i % 2) == 1;

			const glm::vec2 tl = { left, top };
			const glm::vec2 tr = { right, top };
			const glm::vec2 br = { right, bottom };
			const glm::vec2 bl = { left, bottom };

			glm::vec2 bigPos;
			glm::vec2 smallPos;
			glm::vec2 diagA;
			glm::vec2 diagB;
			glm::vec2 yellowCorner;
			float yellowAngleStart = 0.0f;

			if (!mirrored)
			{
				// Big top-left, small bottom-right, diagonal TL→BR, yellow at TR
				bigPos = { left + bigR, top - bigR };
				smallPos = { right - smallR, bottom + smallR };
				diagA = tl;
				diagB = br;
				yellowCorner = tr;
				yellowAngleStart = PI; // left → down
			}
			else
			{
				// Vertical mirror: big bottom-left, small top-right, diagonal BL→TR, yellow at BR
				bigPos = { left + bigR, bottom + bigR };
				smallPos = { right - smallR, top - smallR };
				diagA = bl;
				diagB = tr;
				yellowCorner = br;
				yellowAngleStart = 0.5f * PI; // up → left
			}

			// From big-circle center: project left, and down (or up when mirrored)
			const glm::vec2 leftHit = { left, bigPos.y };
			const glm::vec2 verticalHit = mirrored
				? glm::vec2{ bigPos.x, top }
				: glm::vec2{ bigPos.x, bottom };
			sq.cornerTriangle.create(bigPos, leftHit, verticalHit);
			sq.cornerTriangle.setColor(TRIANGLE_FILL);

			sq.oliveBigUnder.create(bigR * 1.08f);
			sq.oliveBigUnder.setSegmentsCount(40);
			sq.oliveBigUnder.setPosition(bigPos);
			sq.oliveBigUnder.setColor(OLIVE_DARK);

			sq.oliveBig.create(bigR);
			sq.oliveBig.setSegmentsCount(40);
			sq.oliveBig.setPosition(bigPos);
			sq.oliveBig.setColor(OLIVE);

			sq.oliveSmallUnder.create(smallR * 1.12f);
			sq.oliveSmallUnder.setSegmentsCount(32);
			sq.oliveSmallUnder.setPosition(smallPos);
			sq.oliveSmallUnder.setColor(OLIVE_DARK);

			sq.oliveSmall.create(smallR);
			sq.oliveSmall.setSegmentsCount(32);
			sq.oliveSmall.setPosition(smallPos);
			sq.oliveSmall.setColor(OLIVE);

			sq.diagonal.create(diagA, diagB, diagonalThickness);
			sq.diagonal.setColor(BRIGHT_RED);

			std::vector<glm::vec2> quarter;
			quarter.push_back(yellowCorner);
			constexpr int arcSegs = 14;
			for (int s = 0; s <= arcSegs; s++)
			{
				const float t = static_cast<float>(s) / static_cast<float>(arcSegs);
				const float angle = yellowAngleStart + t * (0.5f * PI);
				quarter.push_back({
					yellowCorner.x + yellowR * std::cos(angle),
					yellowCorner.y + yellowR * std::sin(angle)
				});
			}
			sq.yellowQuarter.create(quarter);
			sq.yellowQuarter.setColor(YELLOW_MIST);

			for (int b = 0; b < BLUE_ALONG_DIAGONAL; b++)
			{
				const float t = (static_cast<float>(b) + 0.5f) / static_cast<float>(BLUE_ALONG_DIAGONAL);
				const glm::vec2 pos = diagA + (diagB - diagA) * t;
				sq.blueDots[static_cast<size_t>(b)].create(blueR);
				sq.blueDots[static_cast<size_t>(b)].setSegmentsCount(16);
				sq.blueDots[static_cast<size_t>(b)].setPosition(pos);
				sq.blueDots[static_cast<size_t>(b)].setColor(BLUE_DOT);
			}

			const glm::vec2 edges[4][2] = {
				{ tl, tr }, { tr, br }, { br, bl }, { bl, tl }
			};
			sq.borders.resize(4);
			for (int e = 0; e < 4; e++)
			{
				sq.borders[static_cast<size_t>(e)].create(edges[e][0], edges[e][1], borderThickness);
				sq.borders[static_cast<size_t>(e)].setColor(DARK_RED);
			}
		}
	}

	void PortalMosaic::render() const
	{
		m_background.render();
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

	void PortalMosaic::destroy()
	{
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
		m_background.destroy();
	}

} // namespace Demo
