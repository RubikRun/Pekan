#include "TriangleShape.h"

#include "PekanLogger.h"
#include "Utils/PekanUtils.h"

using namespace Pekan::Graphics;

namespace Pekan
{
namespace Renderer2D
{

#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
    // Checks if the orientation of 3 given vertices is CCW (counter-clockwise)
    static bool isOrientationCCW(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
    {
        const float det = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        return (det >= 0.0f);
    }
#endif

    void TriangleShape::create(glm::vec2 vertexA, glm::vec2 vertexB, glm::vec2 vertexC, bool dynamic)
    {
        Shape::create(dynamic);

        m_verticesLocal[0] = vertexA;
        m_verticesLocal[1] = vertexB;
        m_verticesLocal[2] = vertexC;

#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
        m_needUpdateIndices = true;
#endif
        m_needUpdateVerticesWorld = true;
    }

    void TriangleShape::setVertexA(glm::vec2 vertexA)
    {
        PK_ASSERT(isValid(), "Trying to set vertex A of a TriangleShape that is not yet created.", "Pekan");

        m_verticesLocal[0] = vertexA;
#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
        m_needUpdateIndices = true;
#endif
        m_needUpdateVerticesWorld = true;
    }

    void TriangleShape::setVertexB(glm::vec2 vertexB)
    {
        PK_ASSERT(isValid(), "Trying to set vertex B of a TriangleShape that is not yet created.", "Pekan");

        m_verticesLocal[1] = vertexB;
#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
        m_needUpdateIndices = true;
#endif
        m_needUpdateVerticesWorld = true;
    }

    void TriangleShape::setVertexC(glm::vec2 vertexC)
    {
        PK_ASSERT(isValid(), "Trying to set vertex C of a TriangleShape that is not yet created.", "Pekan");

        m_verticesLocal[2] = vertexC;
#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
        m_needUpdateIndices = true;
#endif
        m_needUpdateVerticesWorld = true;
    }

    void TriangleShape::setVertices(glm::vec2 vertexA, glm::vec2 vertexB, glm::vec2 vertexC)
    {
        PK_ASSERT(isValid(), "Trying to set vertex A of a TriangleShape that is not yet created.", "Pekan");

        m_verticesLocal[0] = vertexA;
        m_verticesLocal[1] = vertexB;
        m_verticesLocal[2] = vertexC;
#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
        m_needUpdateIndices = true;
#endif
        m_needUpdateVerticesWorld = true;
    }

    const ShapeVertex* TriangleShape::getVertices() const
    {
        PK_ASSERT(isValid(), "Trying to get vertices of a TriangleShape that is not yet created.", "Pekan");

#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
        if (m_needUpdateIndices)
        {
            updateIndices();
        }
#endif
        if (m_needUpdateVerticesWorld)
        {
            updateVerticesWorld();
        }
        return m_verticesWorld;
    }

#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
    void TriangleShape::updateIndices() const
    {
        // If face culling is disabled it doesn't matter if triangle is CW or CCW
        // so we can keep indices in whatever order they are.
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

    void TriangleShape::updateVerticesWorld() const
    {
        PK_ASSERT(isValid(), "Trying to update world vertices of a TriangleShape that is not yet created.", "Pekan");

        const glm::mat3& transformMatrix = getTransformMatrix();
        // Calculate world vertex positions by applying the transform matrix to the local vertex positions
        m_verticesWorld[0].position = glm::vec2(transformMatrix * glm::vec3(m_verticesLocal[0], 1.0f));
        m_verticesWorld[1].position = glm::vec2(transformMatrix * glm::vec3(m_verticesLocal[1], 1.0f));
        m_verticesWorld[2].position = glm::vec2(transformMatrix * glm::vec3(m_verticesLocal[2], 1.0f));

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
        // Set "shapeIndex" attribute of each vertex to be shape's index
        m_verticesWorld[0].shapeIndex = m_shapeIndex;
        m_verticesWorld[1].shapeIndex = m_shapeIndex;
        m_verticesWorld[2].shapeIndex = m_shapeIndex;
#else
        // Set "color" attribute of each vertex to be shape's color
        m_verticesWorld[0].color = m_color;
        m_verticesWorld[1].color = m_color;
        m_verticesWorld[2].color = m_color;
#endif

        m_needUpdateVerticesWorld = false;
    }

} // namespace Renderer2D
} // namespace Renderer2D