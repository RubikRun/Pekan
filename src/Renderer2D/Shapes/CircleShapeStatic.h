#pragma once

#include "Shape.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace Pekan
{
namespace Renderer2D
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

		CircleShapeStatic() { generateIndices(); }

		// Creates a circle shape with given radius.
		void create(float radius);
		void destroy() { Shape::_destroy(); }

		void setRadius(float radius);

		inline float getRadius() const { return m_radius; }
		inline int getSegmentsCount() const { return NSegments; }

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		const Vertex2D* getVertices(float shapeIndex) const override;
#else
		const Vertex2D* getVertices() const override;
#endif
		int getVerticesCount() const override { return NSegments; };

		const unsigned* getIndices() const override { return m_indices; }
		int getIndicesCount() const override { return (NSegments - 2) * 3; };

	private: /* functions */

		// Updates local vertices from current radius
		void updateVerticesLocal() const;
		// Updates world vertices from current local vertices and current transform matrix
		void updateVerticesWorld() const;

		// Fills indices array.
		// Since this class has a static number of segments,
		// the indices array will be the same for the whole lifetime of an instance.
		void generateIndices();

	private: /* variables */

		// Vertices making up the circle, in local space
		mutable glm::vec2 m_verticesLocal[NSegments];
		// Vertices making up the circle, in world space
		mutable Vertex2D m_verticesWorld[NSegments];

		// Indices into the vertices list, making up the triangles to be rendered
		unsigned m_indices[(NSegments - 2) * 3];

		// Flag indicating if local vertices need to be updated before use
		mutable bool m_needUpdateVerticesLocal = true;

		// Radius of circle, in local space
		float m_radius = 0.0f;
	};

} // namespace Renderer2D
} // namespace Pekan

#include "CircleShapeStatic_impl.h"
