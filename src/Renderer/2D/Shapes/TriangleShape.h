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
		void create(glm::vec2 vertexA, glm::vec2 vertexB, glm::vec2 vertexC);
		void destroy() { Shape::destroy(); }

		// Sets vertex A/B/C of the triangle, in local space
		void setVertexA(glm::vec2 vertexA);
		void setVertexB(glm::vec2 vertexB);
		void setVertexC(glm::vec2 vertexC);
		// Sets vertices A, B and C of the triangle, in local space
		void setVertices(glm::vec2 vertexA, glm::vec2 vertexB, glm::vec2 vertexC);

		// Returns vertex A/B/C of the triangle, in local space
		inline glm::vec2 getVertexA() const { return m_verticesLocal[0]; }
		inline glm::vec2 getVertexB() const { return m_verticesLocal[1]; }
		inline glm::vec2 getVertexC() const { return m_verticesLocal[2]; }

		const ShapeVertex* getVertices() const override;
		int getVerticesCount() const override { return 3; };

#if !PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING
		const unsigned* getIndices() const override { return m_indices; }
		int getIndicesCount() const override { return 3; };
#endif

	private: /* functions */

#if !PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING
		// Updates indices so that the orientation of the 3 vertices is CCW.
		// This is done only if face culling is enabled in RenderState, otherwise there's no point.
		void updateIndices() const;
#endif

		// Updates world vertices from current local vertices and current transform matrix
		void updateVerticesWorld() const;

	private: /* variables */

		// The 3 vertices (vertex positions) of the triangle, in local space
		glm::vec2 m_verticesLocal[3] = {};
		// The 3 vertices of the triangle, in world space
		mutable ShapeVertex m_verticesWorld[3];

#if !PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING
		// Indices of the 3 vertices of the triangle, determining the order in which they will be drawn
		mutable unsigned m_indices[3] = { 0, 1, 2 };

		// Flag indicating if indices need to be updated before use
		bool m_needUpdateIndices = true;
#endif
	};

} // namespace Renderer
} // namespace Pekan
