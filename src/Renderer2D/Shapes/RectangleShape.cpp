#include "RectangleShape.h"

#include "PekanLogger.h"
#include "Utils/PekanUtils.h"

namespace Pekan
{
namespace Renderer2D
{

    const unsigned RectangleShape::s_indices[6] = { 0, 1, 2, 0, 2, 3 };

    void RectangleShape::create(float width, float height)
    {
        PK_ASSERT(width >= 0.0f, "RectangleShape's width must be greater than or equal to 0.", "Pekan");
        PK_ASSERT(height >= 0.0f, "RectangleShape's height must be greater than or equal to 0.", "Pekan");

        Shape::_create();

        m_width = width;
        m_height = height;
        m_needUpdateVerticesLocal = true;
    }

    void RectangleShape::setWidth(float width)
    {
        PK_ASSERT(isValid(), "Trying to set width of a RectangleShape that is not yet created.", "Pekan");
        PK_ASSERT(width >= 0.0f, "RectangleShape's width must be greater than or equal to 0.", "Pekan");

        m_width = width;
        m_needUpdateVerticesLocal = true;
    }

    void RectangleShape::setHeight(float height)
    {
        PK_ASSERT(isValid(), "Trying to set height of a RectangleShape that is not yet created.", "Pekan");
        PK_ASSERT(height >= 0.0f, "RectangleShape's height must be greater than or equal to 0.", "Pekan");

        m_height = height;
        m_needUpdateVerticesLocal = true;
    }

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
    const Vertex2D* RectangleShape::getVertices(float shapeIndex) const
#else
    const Vertex2D* RectangleShape::getVertices() const
#endif
    {
        PK_ASSERT(isValid(), "Trying to get vertices of a RectangleShape that is not yet created.", "Pekan");

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