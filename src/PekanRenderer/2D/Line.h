#pragma once

#include "RenderObject.h"

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer
{

	// A class representing a 2D line
	class Line
	{
	public:

		// Creates a line between 2 points
		// @param[in] dynamic - Specifies if line is going to be moved often. Used for optimization.
		void create
		(
			glm::vec2 pointA, glm::vec2 pointB,
			bool dynamic = true
		);

		void destroy();

		void render();

		void setPointA(glm::vec2 pointA);
		void setPointB(glm::vec2 pointB);

		inline glm::vec2 getPointA() const { return m_vertices[0]; }
		inline glm::vec2 getPointB() const { return m_vertices[1]; }

		inline glm::vec4 getColor() const { return m_color; }
		void setColor(glm::vec4 color);

	private: /* variables */

		RenderObject m_renderObject;

		// The 2 vertices of the line, in world space
		glm::vec2 m_vertices[2];

		glm::vec4 m_color;
	};

} // namespace Renderer
} // namespace Renderer