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
		void destroy();

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

		int getNumberOfVertices() const override { return 3; }

	protected: /* functions */

		void updateTransformedVertices() override;

	private: /* functions */

		const glm::vec2* getVertexData() const override { return m_verticesWorld; };

#if !PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING
		const unsigned* getIndexData() const override { return m_indices; }

		// Updates indices so that the orientation of the 3 vertices is CCW.
		// This is done only if face culling is enabled in RenderState, otherwise there's no point.
		void updateIndicesOrientation();
#endif

	private: /* variables */

		// The 3 vertices of the triangle, in local space
		glm::vec2 m_verticesLocal[3] = { glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f) };
		// The 3 vertices of the triangle, in world space
		glm::vec2 m_verticesWorld[3] = { glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f) };

#if !PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING
		// Indices of the 3 vertices of the triangle, determining the order in which they will be drawn
		unsigned m_indices[3] = { 0, 1, 2 };
#endif
	};

} // namespace Renderer
} // namespace Renderer