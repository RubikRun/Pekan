#include "RectangleShape.h"

#include "Utils/PekanUtils.h"

namespace Pekan
{
namespace Renderer
{

    const unsigned RectangleShape::s_indices[6] = { 0, 1, 2, 0, 2, 3 };

	void RectangleShape::create
    (
        float width, float height,
        bool dynamic
    )
	{
        m_width = width;
        m_height = height;

        calculateVerticesLocal();
        m_verticesWorld[0] = m_verticesLocal[0];
        m_verticesWorld[1] = m_verticesLocal[1];
        m_verticesWorld[2] = m_verticesLocal[2];
        m_verticesWorld[3] = m_verticesLocal[3];

        Shape::createRenderObject(dynamic);
	}

    void RectangleShape::destroy()
    {
        Shape::destroyRenderObject();
    }

    void RectangleShape::setWidth(float width)
    {
        m_width = width;
        calculateVerticesLocal();
        updateTransformedVertices();
    }

    void RectangleShape::setHeight(float height)
    {
        m_height = height;
        calculateVerticesLocal();
        updateTransformedVertices();
    }

    void RectangleShape::updateTransformedVertices()
    {
        // Multiply local vertices by transform matrix to get world vertices.
        // NOTE: Local vertices are 2D, world vertices are also 2D,
        //       but the transform matrix is 3x3, so we need to convert a local vertex to 3D
        //       by adding a 3rd component of 1.0, then multiply it by the matrix, and then cut out the 3rd component,
        //       to get the final 2D world vertex.
        m_verticesWorld[0] = glm::vec2(m_transformMatrix * glm::vec3(m_verticesLocal[0], 1.0f));
        m_verticesWorld[1] = glm::vec2(m_transformMatrix * glm::vec3(m_verticesLocal[1], 1.0f));
        m_verticesWorld[2] = glm::vec2(m_transformMatrix * glm::vec3(m_verticesLocal[2], 1.0f));
        m_verticesWorld[3] = glm::vec2(m_transformMatrix * glm::vec3(m_verticesLocal[3], 1.0f));

        Shape::updateRenderObject(true, false);
    }

    void RectangleShape::calculateVerticesLocal()
    {
        m_verticesLocal[0] = glm::vec2(-m_width / 2.0f, -m_height / 2.0f);
        m_verticesLocal[1] = glm::vec2( m_width / 2.0f, -m_height / 2.0f);
        m_verticesLocal[2] = glm::vec2( m_width / 2.0f,  m_height / 2.0f);
        m_verticesLocal[3] = glm::vec2(-m_width / 2.0f,  m_height / 2.0f);
    }

} // namespace Renderer
} // namespace Renderer