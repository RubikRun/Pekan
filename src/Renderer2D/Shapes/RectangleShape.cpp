#include "RectangleShape.h"

#include "PekanLogger.h"
#include "Utils/PekanUtils.h"

namespace Pekan
{
namespace Renderer2D
{

    const unsigned RectangleShape::s_indices[6] = { 0, 1, 2, 0, 2, 3 };

    void RectangleShape::create(float width, float height, bool dynamic)
    {
        PK_ASSERT(width > 0.0f, "RectangleShape's width must be greater than 0.", "Pekan");
        PK_ASSERT(height > 0.0f, "RectangleShape's height must be greater than 0.", "Pekan");

        Shape::create(dynamic);

        m_width = width;
        m_height = height;
        m_needUpdateVerticesLocal = true;
    }

    void RectangleShape::setWidth(float width)
    {
        PK_ASSERT(isValid(), "Trying to set width of a RectangleShape that is not yet created.", "Pekan");
        PK_ASSERT(width > 0.0f, "RectangleShape's width must be greater than 0.", "Pekan");

        m_width = width;
        m_needUpdateVerticesLocal = true;
    }

    void RectangleShape::setHeight(float height)
    {
        PK_ASSERT(isValid(), "Trying to set height of a RectangleShape that is not yet created.", "Pekan");
        PK_ASSERT(height > 0.0f, "RectangleShape's height must be greater than 0.", "Pekan");

        m_height = height;
        m_needUpdateVerticesLocal = true;
    }

    const ShapeVertex* RectangleShape::getVertices() const
    {
        PK_ASSERT(isValid(), "Trying to get vertices of a RectangleShape that is not yet created.", "Pekan");

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

    void RectangleShape::updateVerticesLocal() const
    {
        PK_ASSERT(isValid(), "Trying to update local vertices of a RectangleShape that is not yet created.", "Pekan");

        // Use width and height to compute the 4 local vertex positions
        m_verticesLocal[0] = glm::vec2(-m_width / 2.0f, -m_height / 2.0f);
        m_verticesLocal[1] = glm::vec2(m_width / 2.0f, -m_height / 2.0f);
        m_verticesLocal[2] = glm::vec2(m_width / 2.0f, m_height / 2.0f);
        m_verticesLocal[3] = glm::vec2(-m_width / 2.0f, m_height / 2.0f);

        m_needUpdateVerticesLocal = false;
        m_needUpdateVerticesWorld = true;
    }

    void RectangleShape::updateVerticesWorld() const
    {
        PK_ASSERT(isValid(), "Trying to update world vertices of a RectangleShape that is not yet created.", "Pekan");

        const glm::mat3& transformMatrix = getTransformMatrix();
        // Calculate world vertex positions by applying the transform matrix to the local vertex positions
        m_verticesWorld[0].position = glm::vec2(transformMatrix * glm::vec3(m_verticesLocal[0], 1.0f));
        m_verticesWorld[1].position = glm::vec2(transformMatrix * glm::vec3(m_verticesLocal[1], 1.0f));
        m_verticesWorld[2].position = glm::vec2(transformMatrix * glm::vec3(m_verticesLocal[2], 1.0f));
        m_verticesWorld[3].position = glm::vec2(transformMatrix * glm::vec3(m_verticesLocal[3], 1.0f));
        // Set "shapeIndex" attribute of each vertex to be shape's index
        m_verticesWorld[0].shapeIndex = m_shapeIndex;
        m_verticesWorld[1].shapeIndex = m_shapeIndex;
        m_verticesWorld[2].shapeIndex = m_shapeIndex;
        m_verticesWorld[3].shapeIndex = m_shapeIndex;

        m_needUpdateVerticesWorld = false;
    }

} // namespace Renderer2D
} // namespace Renderer2D