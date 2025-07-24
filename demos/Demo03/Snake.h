#pragma once

#include "RenderObject.h"

#include "Apple.h"

namespace Demo
{

	class Snake
	{
	public:

		bool create();

		void update();

		void render() const;

		void destroy();

		// Checks if snake's head is currently overlapping with the given apple
		bool checkHeadOverlapsApple(const Apple& apple) const;

		// Checks if snake's body (including head) is currently overlapping with the given apple
		bool checkBodyOverlapsApple(const Apple& apple) const;

		// Grows the snake by 1 square. Should be called whenever the snake eats an apple.
		void grow();

		inline int getThickness() const { return m_thickness; }

	private: /* functions */

		void move();

		void setSquarePosition(int idx, glm::ivec2 pos);

		glm::ivec2 getSquarePosition(int idx) const;

		glm::ivec4 getRectangle(int idx) const;

		void moveGrowing();
		void moveNormally();

		bool isBitingItself() const;

	private: /* variables */

		Pekan::Renderer2D::RenderObject m_renderObject;

		int m_squaresCount = -1;

		int m_headIdx = -1;

		int m_tailIdx = -1;

		// Vertices of squares that make up snake's body
		std::vector<int> m_vertices;

		// Indices of vertices determining the order of drawing
		std::vector<int> m_indices;

		// Direction to which the snake is moving currently
		glm::ivec2 m_direction = { -1, 0 };

		// Number of frames rendered so far
		int m_frames = 0;

		int m_thickness = 0;

		// Flag used internally to mark the snake for growth,
		// so that it grows the next time when moved.
		bool m_shouldGrow = false;

		int m_speedIdx = 0;

		// Flag indicating if game is still running.
		// When snake bites itself or hits the window's borders
		// the game will end and this flag will be set to false.
		bool m_running = true;

		glm::ivec2 m_resolution;
	};

} // namespace Demo