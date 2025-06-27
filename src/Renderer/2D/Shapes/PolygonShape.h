#pragma once

#include "Shape.h"

#include <glm/glm.hpp>
#include <vector>

namespace Pekan
{
namespace Renderer
{

	// A class representing a 2D polygon shape with a solid color
	class PolygonShape : public Shape
	{
	public:

		// Creates a polygon shape with given vertices
		// @param[in] dynamic - Specifies if polygon is going to be moved often. Used for optimization.
		void create(const std::vector<glm::vec2>& vertices);
		void destroy() { Shape::destroy(); }

		// Sets vertices of polygon, in local space
		void setVertices(const std::vector<glm::vec2>& vertices);
		// Sets a specific vertex of polygon, in local space
		void setVertex(int index, glm::vec2 vertex);

	private: /* functions */

		const ShapeVertex* getVertices() const override;
		int getVerticesCount() const override { return m_verticesLocal.size(); };

		const unsigned* getIndices() const override
		{
			// If indices are empty, this means that polygon is convex,
			// so it didn't need triangulation - that's why we don't have indices.
			if (m_indices.empty())
			{
				return nullptr;
			}
			return m_indices.data();
		}
		int getIndicesCount() const override { return m_indices.size(); };

		DrawMode getDrawMode() const override
		{
			// If indices are empty, this means that polygon is convex,
			// so use triangle fan primitive.
			if (m_indices.empty())
			{
				return DrawMode::TriangleFan;
			}
			// Otherwise use triangle primitive + indices
			return DrawMode::Triangles;
		}

		// Updates local vertices, triangulating and/or reversing them if needed.
		void updateVerticesLocal() const;
		// Updates world vertices from current local vertices and current transform matrix
		void updateVerticesWorld() const;

	private: /* variables */

		// The vertices (vertex positions) of the polygon, in local space
		mutable std::vector<glm::vec2> m_verticesLocal;
		// The vertices of the polygon, in world space
		mutable std::vector<ShapeVertex> m_verticesWorld;

		// Flag indicating if local vertices are reversed.
		mutable bool m_isReversedVerticesLocal = false;

		// Flag indicating if local vertices need to be updated before use
		mutable bool m_needUpdateVerticesLocal = true;

		// Indices into the vertices list, making up the triangles of the triangulated polygon.
		// Used only if polygon is non-convex. 
		mutable std::vector<unsigned> m_indices;
	};

} // namespace Renderer
} // namespace Renderer