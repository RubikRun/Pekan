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
        m_verticesLocal[0] = vertexA;
        m_verticesLocal[1] = vertexB;
        m_verticesLocal[2] = vertexC;
        m_verticesWorld[0] = m_verticesLocal[0];
        m_verticesWorld[1] = m_verticesLocal[1];
        m_verticesWorld[2] = m_verticesLocal[2];

#if !PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING
        updateIndicesOrientation();
#endif

        Shape::createRenderObject(dynamic);
	}

    void TriangleShape::destroy()
    {
        Shape::destroyRenderObject();
    }

    void TriangleShape::setVertexA(glm::vec2 vertexA)
    {
        m_verticesLocal[0] = vertexA;
#if !PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING
        updateIndicesOrientation();
#endif

        updateTransformedVertices();
    }

    void TriangleShape::setVertexB(glm::vec2 vertexB)
    {
        m_verticesLocal[1] = vertexB;
#if !PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING
        updateIndicesOrientation();
#endif

        updateTransformedVertices();
    }

    void TriangleShape::setVertexC(glm::vec2 vertexC)
    {
        m_verticesLocal[2] = vertexC;
#if !PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING
        updateIndicesOrientation();
#endif

        updateTransformedVertices();
    }

    void TriangleShape::setVertices(glm::vec2 vertexA, glm::vec2 vertexB, glm::vec2 vertexC)
    {
        m_verticesLocal[0] = vertexA;
        m_verticesLocal[1] = vertexB;
        m_verticesLocal[2] = vertexC;
#if !PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING
        updateIndicesOrientation();
#endif

        updateTransformedVertices();
    }

    void TriangleShape::updateTransformedVertices()
    {
        // Multiply local vertices by transform matrix to get world vertices.
        // NOTE: Local vertices are 2D, world vertices are also 2D,
        //       but the transform matrix is 3x3, so we need to convert a local vertex to 3D
        //       by adding a 3rd component of 1.0, then multiply it by the matrix, and then cut out the 3rd component,
        //       to get the final 2D world vertex.
        m_verticesWorld[0] = glm::vec2(m_transformMatrix * glm::vec3(m_verticesLocal[0], 1.0f));
        m_verticesWorld[1] = glm::vec2(m_transformMatrix * glm::vec3(m_verticesLocal[1], 1.0f));
        m_verticesWorld[2] = glm::vec2(m_transformMatrix * glm::vec3(m_verticesLocal[2], 1.0f));

        Shape::updateRenderObject();
    }

#if !PEKAN_DISABLE_2D_SHAPES_ORIENTATION_CHECKING
    void TriangleShape::updateIndicesOrientation()
    {
        if (!RenderState::isEnabledFaceCulling())
        {
            return;
        }
        if (isOrientationCCW(m_verticesLocal[0], m_verticesLocal[1], m_verticesLocal[2]))
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