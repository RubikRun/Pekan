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
		std::vector<float> m_vertices;

		glm::vec2 m_position = { 0.0f, 0.0f };

		float m_aspectRatio = 0.0f;
	};

} // namespace Demo