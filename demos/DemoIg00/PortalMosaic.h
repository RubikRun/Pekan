#pragma once

#include "RectangleShape.h"
#include "CircleShape.h"
#include "LineShape.h"
#include "PolygonShape.h"
#include "TriangleShape.h"

#include <vector>
#include <array>

namespace Demo
{

	// Tall portal rendered as stacked arabic-style mosaic squares.
	class PortalMosaic
	{
	public:
		void create(float centerX, float bottomY, float squareSize, int squareCount);
		void render() const;
		void destroy();

		float getCenterY() const { return m_centerY; }
		float getHeight() const { return m_height; }

	private:
		static constexpr int BLUE_ALONG_DIAGONAL = 10;

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

		Pekan::Renderer2D::RectangleShape m_background;
		std::vector<SquareDecor> m_squares;
		float m_centerY = 0.0f;
		float m_height = 0.0f;
	};

} // namespace Demo
