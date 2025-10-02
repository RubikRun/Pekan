#pragma once

#include "Shape.h"

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

	// A class representing a 2D line between two points
	class LineShape : public Shape
	{
	public:

		// Creates a line between 2 points.
		void create(glm::vec2 pointA, glm::vec2 pointB, float thickness = 0.002f);
		void destroy() { Shape::_destroy(); }

		// Sets point A/B of the line, in local space
		void setPointA(glm::vec2 pointA);
		void setPointB(glm::vec2 pointB);
		// Sets line's thickness
		void setThickness(float thickness);

		// Returns point A/B of the line, in local space
		glm::vec2 getPointA() const { return m_pointA; }
		glm::vec2 getPointB() const { return m_pointB; }
		// Returns line's thickness
		float getThickness() const { return m_thickness; }

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		const Vertex2D* getVertices(float shapeIndex) const override;
#else
		const Vertex2D* getVertices() const override;
#endif
		int getVerticesCount() const override { return 4; };

		const unsigned* getIndices() const override { return s_indices; }
		int getIndicesCount() const override { return 6; };

	private: /* functions */

		// Updates local vertices from current point A, point B and thickness
		void updateVerticesLocal() const;
		// Updates world vertices from current local vertices and current transform matrix
		void updateVerticesWorld() const;

	private: /* variables */

		// The 4 vertices (vertex positions) of the rectangle, in local space.
		mutable glm::vec2 m_verticesLocal[4] = {};
		// The 4 vertices of the rectangle, in world space.
		mutable Vertex2D m_verticesWorld[4];

		// Flag indicating if local vertices need to be updated before use
		mutable bool m_needUpdateVerticesLocal = true;

		glm::vec2 m_pointA = glm::vec2(0.0f, 0.0f);
		glm::vec2 m_pointB = glm::vec2(0.0f, 0.0f);
		float m_thickness = -1.0f;

		// Indices of vertices of the 2 triangles making up the line
		static const unsigned s_indices[6];
	};

} // namespace Renderer2D
} // namespace Pekan
