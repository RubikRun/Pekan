#include "RectangleShape.h"

#include "Utils/PekanUtils.h"

namespace Pekan
{
namespace Renderer
{

    // Indices of vertices of the 2 triangles making up the rectangle
    static const unsigned INDICES[6] = { 0, 1, 2, 0, 2, 3 };

	void RectangleShape::create
    (
        float width, float height,
        bool dynamic
    )
	{
        Shape::create();

        m_width = width;
        m_height = height;

        m_vertices[0] = glm::vec2(0.0f, 0.0f);
        m_vertices[1] = glm::vec2(width, 0.0f);
        m_vertices[2] = glm::vec2(width, height);
        m_vertices[3] = glm::vec2(0.0f, height);

        Shape::createRenderObject(m_vertices, INDICES, dynamic);
	}

    void RectangleShape::setWidth(float width)
    {
        m_width = width;
        m_vertices[1].x = width + m_position.x;
        m_vertices[2].x = width + m_position.x;
        updateRenderObject();
    }

    void RectangleShape::setHeight(float height)
    {
        m_height = height;
        m_vertices[2].y = height + m_position.y;
        m_vertices[3].y = height + m_position.y;
        updateRenderObject();
    }

    void RectangleShape::_moveVertices(glm::vec2 deltaPosition)
    {
        m_vertices[0] += deltaPosition;
        m_vertices[1] += deltaPosition;
        m_vertices[2] += deltaPosition;
        m_vertices[3] += deltaPosition;
        updateRenderObject();
    }

} // namespace Renderer
} // namespace Renderer