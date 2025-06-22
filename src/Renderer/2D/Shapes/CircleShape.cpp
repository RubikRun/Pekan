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
        m_radius = radius;
        m_segmentsCount = DEFAULT_SEGMENTS_COUNT;

        generateVerticesLocal();
        m_verticesWorld = m_verticesLocal;

        Shape::createRenderObject(dynamic);
	}

    void CircleShape::destroy()
    {
        Shape::destroyRenderObject();
    }

    void CircleShape::setRadius(float radius)
    {
        m_radius = radius;
        generateVerticesLocal();
        updateTransformedVertices();
    }

    void CircleShape::setSegmentsCount(int segmentsCount)
    {
        m_segmentsCount = segmentsCount;
        generateVerticesLocal();
        m_verticesWorld.resize(m_verticesLocal.size());
        updateTransformedVertices();
    }

    void CircleShape::updateTransformedVertices()
    {
        // Multiply local vertices by transform matrix to get world vertices.
        // NOTE: Local vertices are 2D, world vertices are also 2D,
        //       but the transform matrix is 3x3, so we need to convert a local vertex to 3D
        //       by adding a 3rd component of 1.0, then multiply it by the matrix, and then cut out the 3rd component,
        //       to get the final 2D world vertex.
        for (size_t i = 0; i < m_verticesLocal.size(); i++)
        {
            m_verticesWorld[i] = glm::vec2(m_transformMatrix * glm::vec3(m_verticesLocal[i], 1.0f));
        }

        Shape::updateRenderObject();
    }

    void CircleShape::generateVerticesLocal()
    {
        m_verticesLocal.clear();
        m_verticesLocal.reserve(m_segmentsCount + 2);
        m_verticesLocal.push_back(glm::vec2(0.0f, 0.0f));

        for (int i = 1; i <= m_segmentsCount + 1; i++)
        {
            const float angle = i * 2.0f * PI / m_segmentsCount;
            const float x = m_radius * cos(angle);
            const float y = m_radius * sin(angle);
            m_verticesLocal.push_back({ x, y });
        }
    }

} // namespace Renderer
} // namespace Renderer