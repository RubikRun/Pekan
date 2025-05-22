#include <glm/gtc/constants.hpp>

namespace Pekan
{
namespace Renderer
{

    template <unsigned NSegments>
	void CircleShapeStatic<NSegments>::create
    (
        float radius,
        bool dynamic
    )
	{
        Shape::create();

        m_radius = radius;
        generateVertices();

        Shape::createRenderObject(m_vertices, dynamic);
	}

    template <unsigned NSegments>
    void CircleShapeStatic<NSegments>::setRadius(float radius)
    {
        m_radius = radius;
        generateVertices();
        updateRenderObject();
    }

    template <unsigned NSegments>
    void CircleShapeStatic<NSegments>::_moveVertices(glm::vec2 deltaPosition)
    {
        for (int i = 0; i <= NSegments + 1; i++)
        {
            m_vertices[i] += deltaPosition;
        }

        updateRenderObject();
    }

    template <unsigned NSegments>
    void CircleShapeStatic<NSegments>::generateVertices()
    {
        m_vertices[0] = m_position;

        for (int i = 1; i <= NSegments + 1; i++)
        {
            const float angle = i * 2.0f * glm::pi<float>() / NSegments;
            const float x = m_radius * cos(angle) + m_position.x;
            const float y = m_radius * sin(angle) + m_position.y;
            m_vertices[i] = { x, y };
        }
    }

} // namespace Renderer
} // namespace Renderer