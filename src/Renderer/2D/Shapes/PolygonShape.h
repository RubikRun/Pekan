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
		void create
		(
			const std::vector<glm::vec2>& vertices,
			bool dynamic = true
		);

		void setVertices(const std::vector<glm::vec2>& vertices);
		void setVertex(int index, glm::vec2 vertex);

		inline const std::vector<glm::vec2>& getVertices() const { return m_vertices; }
		glm::vec2 getVertex(int index) const;

		int getNumberOfVertices() const override { return m_vertices.size(); }

	private: /* functions */

		void _moveVertices(glm::vec2 deltaPosition) override;

		const glm::vec2* getVertexData() const override { return m_vertices.data(); };

		virtual DrawMode getDrawMode() const { return DrawMode::TriangleFan; }

#ifndef NDEBUG
		// Checks if polygon is convex
		bool isConvex() const;
#endif

	private: /* variables */

		// The vertices of the polygon, in world space
		std::vector<glm::vec2> m_vertices;
	};

} // namespace Renderer
} // namespace Renderer