#pragma once

#include "Shape.h"

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer
{

	// A class representing a 2D line
	class LineShape : public Shape
	{
	public:

		// Creates a line between 2 points.
		// @param[in] dynamic - Specifies if line is going to be moved often. Used for optimization.
		void create
		(
			glm::vec2 pointA, glm::vec2 pointB, float thickness = 0.002f,
			bool dynamic = true
		);

		void setPointA(glm::vec2 pointA);
		void setPointB(glm::vec2 pointB);
		void setThickness(float thickness);

		inline glm::vec2 getPointA() const { return m_vertices[0]; }
		inline glm::vec2 getPointB() const { return m_vertices[1]; }
		inline float getThickness() const { return m_thickness; }

	private: /* functions */

		void _moveVertices(glm::vec2 deltaPosition) override;

		int getNumberOfVertices() const override { return 4; }

		const glm::vec2* getVertexData() const override { return m_vertices; };

		// Generates (or regenerates) line's vertices based on current point A, point B and thickness
		void generateVertices();

	private: /* variables */

		// The 4 vertices of the line, in world space
		glm::vec2 m_vertices[4] = { glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f) };

		glm::vec2 m_pointA = glm::vec2(0.0f, 0.0f);
		glm::vec2 m_pointB = glm::vec2(0.0f, 0.0f);
		float m_thickness = 0.0f;
	};

} // namespace Renderer
} // namespace Renderer