#pragma once

#include "Shape.h"
#include <vector>
#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

	// A class representing a 2D circle shape with a solid color.
	//
	// NOTE: This class supports dynamically changing the number of segments of a circle,
	//       meaning that you can change the number of segments during the lifetime of a circle object.
	//       If you don't plan on dynamically changing the number of segments it'd be better to instead use
	//           class CircleShapeStatic
	//       for better performance.
	class CircleShape : public Shape
	{
	public:

		// Creates a circle shape with given radius.
		void create(float radius);
		void destroy() { Shape::_destroy(); }

		void setRadius(float radius);
		void setSegmentsCount(int segmentsCount);

		inline float getRadius() const { return m_radius; }
		inline int getSegmentsCount() const { return m_segmentsCount; }

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		const Vertex2D* getVertices(float shapeIndex) const override;
#else
		const Vertex2D* getVertices() const override;
#endif
		int getVerticesCount() const override { return m_verticesLocal.size(); };

		const unsigned* getIndices() const override;
		int getIndicesCount() const override { return m_indices.size(); };

	private: /* functions */

		// Updates local vertices from current radius and segments count
		void updateVerticesLocal() const;
		// Updates world vertices from current local vertices and current transform matrix
		void updateVerticesWorld() const;
		// Updates indices from current segments count
		void updateIndices() const;

	private: /* variables */

		// Vertices (vertex positions) making up the circle, in local space
		mutable std::vector<glm::vec2> m_verticesLocal;
		// Vertices making up the circle, in world space
		mutable std::vector<Vertex2D> m_verticesWorld;

		// Indices into the vertices list, making up the triangles to be rendered
		mutable std::vector<unsigned> m_indices;

		// Flag indicating if local vertices need to be updated before use
		mutable bool m_needUpdateVerticesLocal = true;
		// Flag indicating if indices need to be updated before use
		mutable bool m_needUpdateIndices = true;

		// Radius of circle, in local space
		float m_radius = -1.0f;

		// Number of segments used to approximate the circle
		int m_segmentsCount = 0;
	};

} // namespace Renderer2D
} // namespace Pekan
