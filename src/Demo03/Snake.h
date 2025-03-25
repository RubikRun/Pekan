#pragma once

#include "RenderObject.h"

namespace Demo
{

	class Snake
	{
	public:

		bool init();

		void update();

		void render();

		void exit();

	private: /* functions */

		void move();

		void setSquarePosition(int idx, glm::vec2 pos);

		glm::vec2 getSquarePosition(int idx);

	private: /* variables */

		RenderObject m_renderObject;

		int m_squaresCount = -1;

		int m_headIdx = -1;

		int m_tailIdx = -1;

		// Vertices of squares that make up snake's body
		std::vector<float> m_vertices;

		// Indices of vertices determined the order of drawing
		std::vector<int> m_indices;

		// Direction to which the snake is moving currently
		glm::vec2 m_direction = { -1.0f, 0.0f };

		// Number of frames rendered so far
		int m_frames = 0;

		float m_aspectRatio = 0.0f;
	};

} // namespace Demo