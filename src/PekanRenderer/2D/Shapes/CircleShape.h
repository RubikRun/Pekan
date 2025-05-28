#pragma once

#include "Shape.h"
#include <vector>
#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer
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
		// @param[in] dynamic - Specifies if circle is going to be moved often. Used for optimization.
		void create
		(
			float radius,
			bool dynamic = true
		);

		void setRadius(float radius);
		void setSegmentsCount(int segmentsCount);

		inline float getRadius() const { return m_radius; }
		inline int getSegmentsCount() const { return m_segmentsCount; }

		int getNumberOfVertices() const override { return m_vertices.size(); }

	private: /* functions */

		void _moveVertices(glm::vec2 deltaPosition) override;

		const glm::vec2* getVertexData() const override { return m_vertices.data(); };

		virtual DrawMode getDrawMode() const { return DrawMode::TriangleFan; }

		// Generates (or regenerates) circle's vertices based on current radius and current number of segments
		void generateVertices();

	private: /* variables */

		// Vertices making up the circle, in world space
		std::vector<glm::vec2> m_vertices;

		float m_radius = 0.0f;

		// Number of segments used to approximate the circle
		int m_segmentsCount = 0;
	};

} // namespace Renderer
} // namespace Renderer