#pragma once

#include "RenderObject.h"

namespace Demo
{

	class Apple
	{
	public:

		bool create();

		void update();

		void render();

		void destroy();

	private: /* variables */

		RenderObject m_renderObject;

		// Vertices of apple's square
		std::vector<int> m_vertices;

		glm::ivec2 m_position = { -1, -1 };
	};

} // namespace Demo