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
		void create(float radius);
		void destroy() { Shape::destroy(); }

		void setRadius(float radius);

		inline float getRadius() const { return m_radius; }
		inline int getSegmentsCount() const { return NSegments; }

	private: /* functions */

		const ShapeVertex* getVertices() const override;
		int getVerticesCount() const override { return NSegments + 2; };

		DrawMode getDrawMode() const override { return DrawMode::TriangleFan; }

		// Updates local vertices from current radius
		void updateVerticesLocal() const;
		// Updates world vertices from current local vertices and current transform matrix
		void updateVerticesWorld() const;

	private: /* variables */

		// Vertices making up the circle, in local space
		mutable glm::vec2 m_verticesLocal[NSegments + 2];
		// Vertices making up the circle, in world space
		mutable ShapeVertex m_verticesWorld[NSegments + 2];

		// Flag indicating if local vertices need to be updated before use
		mutable bool m_needUpdateVerticesLocal = true;

		// Radius of circle, in local space
		float m_radius = 0.0f;
	};

} // namespace Renderer
} // namespace Renderer

#include "CircleShapeStatic_impl.h"
