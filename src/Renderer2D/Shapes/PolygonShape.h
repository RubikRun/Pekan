#pragma once

#include "Shape.h"

#include <glm/glm.hpp>
#include <vector>

namespace Pekan
{
namespace Renderer2D
{

	// A class representing a 2D polygon shape with a solid color
	class PolygonShape : public Shape
	{
	public:

		// Creates a polygon shape with given vertices
		void create(const std::vector<glm::vec2>& vertices);
		void destroy() { Shape::_destroy(); }

		// Sets vertices of polygon, in local space
		void setVertices(const std::vector<glm::vec2>& vertices);
		// Sets a specific vertex of polygon, in local space
		void setVertex(int index, glm::vec2 vertex);

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		const Vertex2D* getVertices(float shapeIndex) const override;
#else
		const Vertex2D* getVertices() const override;
#endif
		int getVerticesCount() const override { return m_verticesLocal.size(); };

		const unsigned* getIndices() const override;
		int getIndicesCount() const override { return m_indices.size(); };

	private: /* functions */

		// Updates local vertices, triangulating and/or reversing them if needed.
		void updateVerticesLocal() const;
		// Updates world vertices from current local vertices and current transform matrix
		void updateVerticesWorld() const;

	private: /* variables */

		// The vertices (vertex positions) of the polygon, in local space
		mutable std::vector<glm::vec2> m_verticesLocal;
		// The vertices of the polygon, in world space
		mutable std::vector<Vertex2D> m_verticesWorld;

		// Indices into the vertices list, making up the triangles of the triangulated polygon.
		mutable std::vector<unsigned> m_indices;

		// Flag indicating if local vertices need to be updated before use
		mutable bool m_needUpdateVerticesLocal = true;

		// Flag indicating if local vertices are reversed.
		mutable bool m_isReversedVerticesLocal = false;

		// Flag indicating if indices are "reversed",
		// or more precisely each 3 consecutive indices have reversed orientation.
		mutable bool m_isReversedIndices = false;

		// Flag indicating if indices list currently contains "triangle fan" indices,
		// meaning indices in the form { 0, 1, 2, 0, 2, 3, 0, 3, 4, ... }
		mutable bool m_isIndicesTriangleFan = false;
	};

} // namespace Renderer2D
} // namespace Pekan
