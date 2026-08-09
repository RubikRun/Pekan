#pragma once

#include "CircleShape.h"
#include "LineShape.h"
#include "PolygonShape.h"
#include "TriangleShape.h"
#include "RenderObject.h"

#include <vector>
#include <array>

namespace Demo
{

	// Tall portal rendered as stacked arabic-style mosaic squares.
	class PortalMosaic
	{
	public:
		void create(float centerX, float bottomY, float squareSize, int squareCount);
		void update(float dt);
		void render() const;
		void destroy();

		float getCenterY() const { return m_centerY; }
		float getHeight() const { return m_height; }

	private:
		static constexpr int BLUE_ALONG_DIAGONAL = 10;
		static constexpr float SCROLL_SPEED = 1.5f;

		struct SquareDecor
		{
			Pekan::Renderer2D::TriangleShape cornerTriangle;
			Pekan::Renderer2D::CircleShape oliveBigUnder;
			Pekan::Renderer2D::CircleShape oliveBig;
			Pekan::Renderer2D::CircleShape oliveSmallUnder;
			Pekan::Renderer2D::CircleShape oliveSmall;
			Pekan::Renderer2D::LineShape diagonal;
			Pekan::Renderer2D::PolygonShape yellowQuarter;
			std::array<Pekan::Renderer2D::CircleShape, BLUE_ALONG_DIAGONAL> blueDots{};
			std::vector<Pekan::Renderer2D::LineShape> borders;
		};

		float opacityAtY(float y) const;
		void layoutSquare(SquareDecor& sq, float bottom, bool mirrored);
		void layoutAll();
		void enablePortalScissor() const;
		void disablePortalScissor() const;
		void renderBackground() const;
		void renderMosaicShapes() const;

		mutable Pekan::Graphics::RenderObject m_background;
		std::vector<SquareDecor> m_squares;

		float m_centerX = 0.0f;
		float m_bottomY = 0.0f;
		float m_centerY = 0.0f;
		float m_height = 0.0f;
		float m_squareSize = 0.0f;
		int m_squareCount = 0;
		float m_scroll = 0.0f;

		float m_bigR = 0.0f;
		float m_smallR = 0.0f;
		float m_yellowR = 0.0f;
		float m_blueR = 0.0f;
		float m_borderThickness = 0.014f;
		float m_diagonalThickness = 0.022f;
	};

} // namespace Demo
