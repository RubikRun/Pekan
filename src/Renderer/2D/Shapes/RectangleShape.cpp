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

        m_vertices[0] = glm::vec2(0.0f, 0.0f);
        m_vertices[1] = glm::vec2(width, 0.0f);
        m_vertices[2] = glm::vec2(width, height);
        m_vertices[3] = glm::vec2(0.0f, height);

        Shape::createRenderObject(dynamic);
	}

    void RectangleShape::destroy()
    {
        Shape::destroyRenderObject();
    }

    void RectangleShape::setWidth(float width)
    {
        m_width = width;
        m_vertices[1].x = width + m_position.x;
        m_vertices[2].x = width + m_position.x;
        Shape::updateRenderObject();
    }

    void RectangleShape::setHeight(float height)
    {
        m_height = height;
        m_vertices[2].y = height + m_position.y;
        m_vertices[3].y = height + m_position.y;
        Shape::updateRenderObject();
    }

    void RectangleShape::_moveVertices(glm::vec2 deltaPosition)
    {
        m_vertices[0] += deltaPosition;
        m_vertices[1] += deltaPosition;
        m_vertices[2] += deltaPosition;
        m_vertices[3] += deltaPosition;
        Shape::updateRenderObject();
    }

} // namespace Renderer
} // namespace Renderer