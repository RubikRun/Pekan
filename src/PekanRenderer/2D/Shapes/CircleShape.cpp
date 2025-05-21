#include "CircleShape.h"

#include "Utils/PekanUtils.h"
#include <glm/gtc/constants.hpp>

static const int DEFAULT_SEGMENTS_COUNT = 42;
static const float PI = glm::pi<float>();

namespace Pekan
{
namespace Renderer
{

	void CircleShape::create
    (
        float radius,
        bool dynamic
    )
	{
        Shape::create();

        m_radius = radius;
        m_segmentsCount = DEFAULT_SEGMENTS_COUNT;

        generateVertices();

        Shape::createRenderObject(m_vertices.data(), dynamic);
	}

    void CircleShape::setRadius(float radius)
    {
        m_radius = radius;
        generateVertices();
        updateRenderObject();
    }

    void CircleShape::setSegmentsCount(int segmentsCount)
    {
        m_segmentsCount = segmentsCount;
        generateVertices();
        updateRenderObject();
    }

    void CircleShape::_moveVertices(glm::vec2 deltaPosition)
    {
        for (int i = 0; i <= m_segmentsCount + 1; i++)
        {
            m_vertices[i] += deltaPosition;
        }

        updateRenderObject();
    }

    void CircleShape::generateVertices()
    {
        m_vertices.clear();
        m_vertices.reserve(m_segmentsCount + 2);
        m_vertices.push_back(m_position);

        for (int i = 0; i <= m_segmentsCount; i++)
        {
            const float angle = i * 2.0f * PI / m_segmentsCount;
            const float x = m_radius * cos(angle) + m_position.x;
            const float y = m_radius * sin(angle) + m_position.y;
            m_vertices.push_back({ x, y });
        }
    }

} // namespace Renderer
} // namespace Renderer