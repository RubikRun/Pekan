#include "TriangleShape.h"

#include "Utils/PekanUtils.h"

namespace Pekan
{
namespace Renderer
{

	void TriangleShape::create
    (
        glm::vec2 vertexA, glm::vec2 vertexB, glm::vec2 vertexC,
        bool dynamic
    )
	{
        Shape::create();

        m_vertices[0] = vertexA;
        m_vertices[1] = vertexB;
        m_vertices[2] = vertexC;

        Shape::createRenderObject(m_vertices, dynamic);
	}

    void TriangleShape::setVertexA(glm::vec2 vertexA)
    {
        m_vertices[0] = vertexA + m_position;
        updateRenderObject();
    }

    void TriangleShape::setVertexB(glm::vec2 vertexB)
    {
        m_vertices[1] = vertexB + m_position;
        updateRenderObject();
    }

    void TriangleShape::setVertexC(glm::vec2 vertexC)
    {
        m_vertices[2] = vertexC + m_position;
        updateRenderObject();
    }

    void TriangleShape::_moveVertices(glm::vec2 deltaPosition)
    {
        m_vertices[0] += deltaPosition;
        m_vertices[1] += deltaPosition;
        m_vertices[2] += deltaPosition;

        updateRenderObject();
    }

    void TriangleShape::updateRenderObject()
    {
        m_renderObject.setVertexData(m_vertices, getVerticesSize());
    }

} // namespace Renderer
} // namespace Renderer