#pragma once

#include "Shape.h"

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer
{

	// A class representing a 2D rectangle shape with a solid color
	class RectangleShape : public Shape
	{
	public:

		// Creates a rectangle shape with given width and height.
		void create(float width, float height);
		void destroy() { Shape::destroy(); }

		void setWidth(float width);
		void setHeight(float height);

		inline float getWidth() const { return m_width; }
		inline float getHeight() const { return m_height; }

		const ShapeVertex* getVertices() const override;
		int getVerticesCount() const override { return 4; };

		const unsigned* getIndices() const override { return s_indices; }
		int getIndicesCount() const override { return 6; };

	private: /* functions */

		// Updates local vertices from current width and height
		void updateVerticesLocal() const;
		// Updates world vertices from current local vertices and current transform matrix
		void updateVerticesWorld() const;

	private: /* variables */

		// The 4 vertices (vertex positions) of the rectangle, in local space.
		mutable glm::vec2 m_verticesLocal[4] = {};
		// The 4 vertices of the rectangle, in world space.
		mutable ShapeVertex m_verticesWorld[4];

		// Flag indicating if local vertices need to be updated before use
		mutable bool m_needUpdateVerticesLocal = true;

		// Width of rectangle, size across the X axis in local space
		float m_width = -1.0f;
		// Height of rectangle, size across the Y acis in local space
		float m_height = -1.0f;

		// Indices of vertices of the 2 triangles making up the rectangle
		static const unsigned s_indices[6];
	};

} // namespace Renderer
} // namespace Pekan
