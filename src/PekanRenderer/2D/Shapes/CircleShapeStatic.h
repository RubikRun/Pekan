#pragma once

#include "Shape.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace Pekan
{
namespace Renderer
{

	// A class representing a 2D circle shape with a solid color.
	// 
	// NOTE: This class has a static number of segments,
	// meaning that once you create a circle with some number of segments,
	// this number cannot be changed during the lifetime of the circle object.
	template <unsigned NSegments = 42>
	class CircleShapeStatic : public Shape
	{
	public:

		// Creates a circle shape with given radius.
		// @param[in] dynamic - Specifies if circle is going to be moved often. Used for optimization.
		void create
		(
			float radius,
			bool dynamic = true
		);

		void setRadius(float radius);

		inline float getRadius() const { return m_radius; }
		inline int getSegmentsCount() const { return NSegments; }

		int getNumberOfVertices() const override { return NSegments + 2; }

	private: /* functions */

		void _moveVertices(glm::vec2 deltaPosition) override;

		const glm::vec2* getVertexData() const override { return m_vertices; };

		virtual DrawMode getDrawMode() const { return DrawMode::TriangleFan; }

		// Generates (or regenerates) circle's vertices based on current radius
		void generateVertices();

	private: /* variables */

		// Vertices making up the circle, in world space
		glm::vec2 m_vertices[NSegments + 2];

		float m_radius = 0.0f;
	};

} // namespace Renderer
} // namespace Renderer

#include "CircleShapeStatic_impl.h"
