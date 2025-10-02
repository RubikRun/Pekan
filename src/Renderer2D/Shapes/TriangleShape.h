#pragma once

#include "Shape.h"

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

	// A class representing a 2D triangle shape with a solid color
	class TriangleShape : public Shape
	{
	public:

		// Creates a triangle shape from given positions of 3 vertices.
		void create(glm::vec2 vertexA, glm::vec2 vertexB, glm::vec2 vertexC);
		void destroy() { Shape::_destroy(); }

		// Sets vertex A/B/C of the triangle, in local space
		void setVertexA(glm::vec2 vertexA);
		void setVertexB(glm::vec2 vertexB);
		void setVertexC(glm::vec2 vertexC);
		// Sets vertices A, B and C of the triangle, in local space
		void setVertices(glm::vec2 vertexA, glm::vec2 vertexB, glm::vec2 vertexC);

		// Returns vertex A/B/C of the triangle, in local space
		glm::vec2 getVertexA() const { return m_verticesLocal[0]; }
		glm::vec2 getVertexB() const { return m_verticesLocal[1]; }
		glm::vec2 getVertexC() const { return m_verticesLocal[2]; }

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		const Vertex2D* getVertices(float shapeIndex) const override;
#else
		const Vertex2D* getVertices() const override;
#endif
		int getVerticesCount() const override { return 3; };

		const unsigned* getIndices() const override;
		int getIndicesCount() const override { return 3; };

	private: /* functions */

#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
		// Updates indices if needed
		void updateIndices() const;
#endif

		// Updates world vertices from current local vertices and current transform matrix
		void updateVerticesWorld() const;

	private: /* variables */

		// The 3 vertices (vertex positions) of the triangle, in local space
		glm::vec2 m_verticesLocal[3] = {};
		// The 3 vertices of the triangle, in world space
		mutable Vertex2D m_verticesWorld[3];

#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
		// Indices of the 3 vertices of the triangle, determining the order in which they will be drawn
		mutable unsigned m_indices[3] = { 0, 1, 2 };
		// Flag indicating if indices need to be updated before use
		bool m_needUpdateIndices = true;
#else
		// Indices of the 3 vertices of the triangle, determining the order in which they will be drawn
		static const unsigned s_indices[3];
#endif
	};

} // namespace Renderer2D
} // namespace Pekan
