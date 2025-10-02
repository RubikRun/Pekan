#include "TriangleShape.h"

#include "PekanLogger.h"
#include "Utils/PekanUtils.h"
#include "Utils/MathUtils.h"
#include "RenderState.h"

using namespace Pekan::Graphics;

namespace Pekan
{
namespace Renderer2D
{

#if !PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
    const unsigned TriangleShape::s_indices[3] = { 0, 1, 2 };
#endif

    void TriangleShape::create(glm::vec2 vertexA, glm::vec2 vertexB, glm::vec2 vertexC)
    {
        Shape::_create();

        m_verticesLocal[0] = vertexA;
        m_verticesLocal[1] = vertexB;
        m_verticesLocal[2] = vertexC;

        m_needUpdateVerticesWorld = true;
#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
        m_needUpdateIndices = true;
#endif
    }

    void TriangleShape::setVertexA(glm::vec2 vertexA)
    {
        PK_ASSERT(isValid(), "Trying to set vertex A of a TriangleShape that is not yet created.", "Pekan");

        m_verticesLocal[0] = vertexA;
        m_needUpdateVerticesWorld = true;
#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
        m_needUpdateIndices = true;
#endif
    }

    void TriangleShape::setVertexB(glm::vec2 vertexB)
    {
        PK_ASSERT(isValid(), "Trying to set vertex B of a TriangleShape that is not yet created.", "Pekan");

        m_verticesLocal[1] = vertexB;
        m_needUpdateVerticesWorld = true;
#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
        m_needUpdateIndices = true;
#endif
    }

    void TriangleShape::setVertexC(glm::vec2 vertexC)
    {
        PK_ASSERT(isValid(), "Trying to set vertex C of a TriangleShape that is not yet created.", "Pekan");

        m_verticesLocal[2] = vertexC;
        m_needUpdateVerticesWorld = true;
#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
        m_needUpdateIndices = true;
#endif
    }

    void TriangleShape::setVertices(glm::vec2 vertexA, glm::vec2 vertexB, glm::vec2 vertexC)
    {
        PK_ASSERT(isValid(), "Trying to set vertex A of a TriangleShape that is not yet created.", "Pekan");

        m_verticesLocal[0] = vertexA;
        m_verticesLocal[1] = vertexB;
        m_verticesLocal[2] = vertexC;
        m_needUpdateVerticesWorld = true;
#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
        m_needUpdateIndices = true;
#endif
    }

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
    const Vertex2D* TriangleShape::getVertices(float shapeIndex) const
#else
    const Vertex2D* TriangleShape::getVertices() const
#endif
    {
        PK_ASSERT(isValid(), "Trying to get vertices of a TriangleShape that is not yet created.", "Pekan");

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
        PK_ASSERT(shapeIndex >= 0.0f, "Trying to get vertices of a Shape but giving it a negative shape index.", "Pekan");
        if (m_shapeIndex != shapeIndex)
        {
            m_shapeIndex = shapeIndex;
            m_needUpdateVerticesWorld = true;
        }
#endif

        if (m_transformChangeIdUsedInVerticesWorld < Transformable2D::getChangeId())
        {
            m_needUpdateVerticesWorld = true;
        }

        if (m_needUpdateVerticesWorld)
        {
            updateVerticesWorld();
        }
        return m_verticesWorld;
    }

    const unsigned* TriangleShape::getIndices() const
    {
        PK_ASSERT(isValid(), "Trying to get indices of a TriangleShape that is not yet created.", "Pekan");

#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
        if (m_needUpdateIndices)
        {
            updateIndices();
        }
        return m_indices;
#else
        return s_indices;
#endif
    }

#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
    void TriangleShape::updateIndices() const
    {
        if (m_transformChangeIdUsedInVerticesWorld < Transformable2D::getChangeId())
        {
            m_needUpdateVerticesWorld = true;
        }

        // If face culling is disabled it doesn't matter if triangle is CW or CCW
        // so we can keep indices in whatever order they are.
        if (!RenderState::isEnabledFaceCulling())
        {
            return;
        }
        // We need latest world vertices to correctly update indices
        if (m_needUpdateVerticesWorld)
        {
            updateVerticesWorld();
        }

        // If the orientation of the 3 vertices in world space is CCW we need standard indices { 0, 1, 2 }
        if (MathUtils::isOrientationCCW(m_verticesWorld[0].position, m_verticesWorld[1].position, m_verticesWorld[2].position))
        {
            m_indices[0] = 0;
            m_indices[2] = 2;
        }
        // Otherwise we need reverse orientation indices { 2, 1, 0 }
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

        const glm::mat3& worldMatrix = getWorldMatrix();
        // Calculate world vertex positions by applying the transform matrix to the local vertex positions
        m_verticesWorld[0].position = glm::vec2(worldMatrix * glm::vec3(m_verticesLocal[0], 1.0f));
        m_verticesWorld[1].position = glm::vec2(worldMatrix * glm::vec3(m_verticesLocal[1], 1.0f));
        m_verticesWorld[2].position = glm::vec2(worldMatrix * glm::vec3(m_verticesLocal[2], 1.0f));

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

        // Cache change ID of the transform that we just used to update world vertices
        m_transformChangeIdUsedInVerticesWorld = Transformable2D::getChangeId();

        m_needUpdateVerticesWorld = false;
    }

} // namespace Renderer2D
} // namespace Renderer2D