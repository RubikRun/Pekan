#include "TriangleShape.h"

#include "Utils/PekanUtils.h"

namespace Pekan
{
namespace Renderer
{

#if !PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING
    // Checks if the orientation of 3 given vertices is CCW (counter-clockwise)
    static bool isOrientationCCW(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
    {
        const float det = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        return (det >= 0.0f);
    }
#endif

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

#if !PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING
        updateIndicesOrientation();
#endif

        Shape::createRenderObject(dynamic);
	}

    void TriangleShape::setVertexA(glm::vec2 vertexA)
    {
        m_vertices[0] = vertexA + m_position;
#if !PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING
        updateIndicesOrientation();
        updateRenderObject(m_indices);
#else
        updateRenderObject();
#endif
    }

    void TriangleShape::setVertexB(glm::vec2 vertexB)
    {
        m_vertices[1] = vertexB + m_position;
#if !PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING
        updateIndicesOrientation();
        updateRenderObject(m_indices);
#else
        updateRenderObject();
#endif
    }

    void TriangleShape::setVertexC(glm::vec2 vertexC)
    {
        m_vertices[2] = vertexC + m_position;
#if !PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING
        updateIndicesOrientation();
        updateRenderObject(m_indices);
#else
        updateRenderObject();
#endif
    }

    void TriangleShape::setVertices(glm::vec2 vertexA, glm::vec2 vertexB, glm::vec2 vertexC)
    {
        m_vertices[0] = vertexA + m_position;
        m_vertices[1] = vertexB + m_position;
        m_vertices[2] = vertexC + m_position;
#if !PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING
        updateIndicesOrientation();
        updateRenderObject(m_indices);
#else
        updateRenderObject();
#endif
    }

    void TriangleShape::_moveVertices(glm::vec2 deltaPosition)
    {
        m_vertices[0] += deltaPosition;
        m_vertices[1] += deltaPosition;
        m_vertices[2] += deltaPosition;
        updateRenderObject();
    }

#if !PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING
    void TriangleShape::updateIndicesOrientation()
    {
        if (!PekanRenderer::isEnabledFaceCulling())
        {
            return;
        }
        if (isOrientationCCW(m_vertices[0], m_vertices[1], m_vertices[2]))
        {
            m_indices[0] = 0;
            m_indices[2] = 2;
        }
        else
        {
            m_indices[0] = 2;
            m_indices[2] = 0;
        }
    }
#endif

} // namespace Renderer
} // namespace Renderer