#include "LineShape.h"

#include "PekanLogger.h"
#include "Utils/PekanUtils.h"

namespace Pekan
{
namespace Renderer2D
{

    const unsigned LineShape::s_indices[6] = { 0, 1, 2, 0, 2, 3 };

    void LineShape::create(glm::vec2 pointA, glm::vec2 pointB, float thickness)
    {
        PK_ASSERT(thickness > 0.0f, "LineShape's thickness must be greater than 0.", "Pekan");

        Shape::_create();

        m_pointA = pointA;
        m_pointB = pointB;
        m_thickness = thickness;
        m_needUpdateVerticesLocal = true;
    }

    void LineShape::setPointA(glm::vec2 pointA)
    {
        PK_ASSERT(isValid(), "Trying to set point A of a LineShape that is not yet created.", "Pekan");
        m_pointA = pointA;
        m_needUpdateVerticesLocal = true;
    }

    void LineShape::setPointB(glm::vec2 pointB)
    {
        PK_ASSERT(isValid(), "Trying to set point B of a LineShape that is not yet created.", "Pekan");
        m_pointB = pointB;
        m_needUpdateVerticesLocal = true;
    }

    void LineShape::setThickness(float thickness)
    {
        PK_ASSERT(isValid(), "Trying to set thickness of a LineShape that is not yet created.", "Pekan");
        PK_ASSERT(thickness >= 0.0f, "LineShape's thickness must be greater than or equal to 0.", "Pekan");

        m_thickness = thickness;
        m_needUpdateVerticesLocal = true;
    }

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
    const Vertex2D* LineShape::getVertices(float shapeIndex) const
#else
    const Vertex2D* LineShape::getVertices() const
#endif
    {
        PK_ASSERT(isValid(), "Trying to get vertices of a LineShape that is not yet created.", "Pekan");

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
        PK_ASSERT(shapeIndex >= 0.0f, "Trying to get vertices of a Shape but giving it a negative shape index.", "Pekan");
        if (m_shapeIndex != shapeIndex)
        {
            m_shapeIndex = shapeIndex;
            m_needUpdateVerticesWorld = true;
        }
#endif

        if (m_needUpdateVerticesLocal)
        {
            updateVerticesLocal();
        }
        if (m_needUpdateVerticesWorld)
        {
            updateVerticesWorld();
        }
        return m_verticesWorld;
    }

    void LineShape::updateVerticesLocal() const
    {
        PK_ASSERT(isValid(), "Trying to update local vertices of a LineShape that is not yet created.", "Pekan");

        // Calculate the normal offset vector.
        // We'll use that vector to "thicken" the line to the required thickness.
        // 
        // The ideal line AB has 0 thickness.
        // To make it have thickness T we need to turn the 2 points A and B into 4 points.
        // Point A will turn into 2 points, and point B will turn into 2 points.
        // The way to turn an original point into 2 points is to add a vector of length T / 2 in the 2 directions perpendicular to AB.
        // The normal offset vector is just that - a vector in one of the perpendicular directions with length T / 2.
        // The vector in the other perpendicular direction will be just the negative of that vector
        const glm::vec2 dir = glm::normalize(m_pointB - m_pointA);
        const glm::vec2 normalOffset
        (
            -(m_thickness / 2.0f) * dir.y,
            (m_thickness / 2.0f) * dir.x
        );
        // To get the 4 vertices of the line we can add and subtract the normal offset vector to A and B.
        m_verticesLocal[0] = m_pointA + normalOffset;
        m_verticesLocal[1] = m_pointA - normalOffset;
        m_verticesLocal[2] = m_pointB - normalOffset;
        m_verticesLocal[3] = m_pointB + normalOffset;

        m_needUpdateVerticesLocal = false;
        m_needUpdateVerticesWorld = true;
    }

    void LineShape::updateVerticesWorld() const
    {
        PK_ASSERT(isValid(), "Trying to update world vertices of a LineShape that is not yet created.", "Pekan");

        const glm::mat3& transformMatrix = getTransformMatrix();
        // Calculate world vertex positions by applying the transform matrix to the local vertex positions
        m_verticesWorld[0].position = glm::vec2(transformMatrix * glm::vec3(m_verticesLocal[0], 1.0f));
        m_verticesWorld[1].position = glm::vec2(transformMatrix * glm::vec3(m_verticesLocal[1], 1.0f));
        m_verticesWorld[2].position = glm::vec2(transformMatrix * glm::vec3(m_verticesLocal[2], 1.0f));
        m_verticesWorld[3].position = glm::vec2(transformMatrix * glm::vec3(m_verticesLocal[3], 1.0f));

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
        // Set "shapeIndex" attribute of each vertex to be shape's index
        m_verticesWorld[0].shapeIndex = m_shapeIndex;
        m_verticesWorld[1].shapeIndex = m_shapeIndex;
        m_verticesWorld[2].shapeIndex = m_shapeIndex;
        m_verticesWorld[3].shapeIndex = m_shapeIndex;
#else
        // Set "color" attribute of each vertex to be shape's color
        m_verticesWorld[0].color = m_color;
        m_verticesWorld[1].color = m_color;
        m_verticesWorld[2].color = m_color;
        m_verticesWorld[3].color = m_color;
#endif

        m_needUpdateVerticesWorld = false;
    }

} // namespace Renderer2D
} // namespace Renderer2D