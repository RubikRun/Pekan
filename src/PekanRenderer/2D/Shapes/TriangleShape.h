#pragma once

#include "Shape.h"

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer
{

	// A class representing a 2D triangle shape with a solid color
	class TriangleShape : public Shape
	{
	public:

		// Creates a triangle shape from given positions of 3 vertices.
		// @param[in] dynamic - Specifies if triangle is going to be moved often. Used for optimization.
		void create
		(
			glm::vec2 vertexA, glm::vec2 vertexB, glm::vec2 vertexC,
			bool dynamic = true
		);

		void setVertexA(glm::vec2 vertexA);
		void setVertexB(glm::vec2 vertexB);
		void setVertexC(glm::vec2 vertexC);

		inline glm::vec2 getVertexA() const { return m_vertices[0]; }
		inline glm::vec2 getVertexB() const { return m_vertices[1]; }
		inline glm::vec2 getVertexC() const { return m_vertices[2]; }

		int getNumberOfVertices() const override { return 3; }

	private: /* functions */

		void _moveVertices(glm::vec2 deltaPosition) override;

		const glm::vec2* getVertexData() const override { return m_vertices; };

	private: /* variables */

		// The 3 vertices of the triangle, in world space
		glm::vec2 m_vertices[3] = { glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f) };
	};

} // namespace Renderer
} // namespace Renderer