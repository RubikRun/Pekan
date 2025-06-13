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
		// @param[in] dynamic - Specifies if rectangle is going to be moved often. Used for optimization.
		void create
		(
			float width, float height,
			bool dynamic = true
		);

		void setWidth(float width);
		void setHeight(float height);

		inline float getWidth() const { return m_width; }
		inline float getHeight() const { return m_height; }

		int getNumberOfVertices() const override { return 4; }

	private: /* functions */

		void _moveVertices(glm::vec2 deltaPosition) override;

		const glm::vec2* getVertexData() const override { return m_vertices; };
		const unsigned* getIndexData() const override { return s_indices; }

	private: /* variables */

		// The 4 vertices of the rectangle, in world space
		glm::vec2 m_vertices[4] = { glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f) };

		float m_width = 0.0f;
		float m_height = 0.0f;

		// Indices of vertices of the 2 triangles making up the rectangle
		static const unsigned s_indices[6];
	};

} // namespace Renderer
} // namespace Renderer