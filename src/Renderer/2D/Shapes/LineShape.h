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
		void destroy();

		// Sets point A/B of the line, in local space
		void setPointA(glm::vec2 pointA);
		void setPointB(glm::vec2 pointB);
		// Sets line's thickness
		void setThickness(float thickness);

		// Returns point A/B of the line, in local space
		inline glm::vec2 getPointA() const { return m_verticesLocal[0]; }
		inline glm::vec2 getPointB() const { return m_verticesLocal[1]; }
		// Returns line's thickness
		inline float getThickness() const { return m_thickness; }

		int getNumberOfVertices() const override { return 4; }

	protected: /* functions */

		void updateTransformedVertices() override;

	private: /* functions */

		const glm::vec2* getVertexData() const override { return m_verticesWorld; };
		const unsigned* getIndexData() const override { return s_indices; }

		// (Re)generates line's local vertices based on current point A, point B and thickness
		void generateVerticesLocal();

	private: /* variables */

		// The 4 vertices of the line, in local space
		glm::vec2 m_verticesLocal[4] = { glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f) };
		// The 4 vertices of the line, in world space
		glm::vec2 m_verticesWorld[4] = { glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f) };

		glm::vec2 m_pointA = glm::vec2(0.0f, 0.0f);
		glm::vec2 m_pointB = glm::vec2(0.0f, 0.0f);
		float m_thickness = 0.0f;

		// Indices of vertices of the 2 triangles making up the line
		static const unsigned s_indices[6];
	};

} // namespace Renderer
} // namespace Renderer