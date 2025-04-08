#pragma once

#include "RenderObject.h"

namespace Demo
{
	class Snake;

	class Apple
	{
	public:

		bool create(const Snake& snake);

		void update();

		void render();

		void destroy();

		glm::ivec4 getRectangle() const;

		// Regenerates the apple at a new random position.
		// More specifically, such position as to not be inside of the given snake.
		void regenerate(const Snake& snake);

	private: /* variables */

		RenderObject m_renderObject;

		// Vertices of apple's square
		std::vector<int> m_vertices;

		glm::ivec2 m_position = { -1, -1 };
		int m_size = -1;

		glm::ivec2 m_resolution;
	};

} // namespace Demo