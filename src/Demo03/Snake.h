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

		void render();

		void destroy();

		// Checks if snake's head is currently overlapping with the given apple
		bool checkHeadOverlapsApple(const Apple& apple) const;

		// Checks if snake's body (including head) is currently overlapping with the given apple
		bool checkBodyOverlapsApple(const Apple& apple) const;

	private: /* functions */

		void move();

		void setSquarePosition(int idx, glm::ivec2 pos);

		glm::ivec2 getSquarePosition(int idx);

		glm::ivec4 getRectangle(int idx) const;

	private: /* variables */

		RenderObject m_renderObject;

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
	};

} // namespace Demo