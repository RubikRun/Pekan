#include "CircleShape.h"

#include "PekanLogger.h"
#include "Utils/PekanUtils.h"
#include "Utils/MathUtils.h"
#include <glm/gtc/constants.hpp>

static const int DEFAULT_SEGMENTS_COUNT = 42;
static const float PI = glm::pi<float>();

namespace Pekan
{
namespace Renderer2D
{

    void CircleShape::create(float radius)
    {
        PK_ASSERT(radius > 0.0f, "CircleShape's radius must be greater than 0.", "Pekan");

        Shape::_create();

        m_radius = radius;
        m_segmentsCount = DEFAULT_SEGMENTS_COUNT;
        m_needUpdateVerticesLocal = true;
        m_needUpdateIndices = true;
	}

    void CircleShape::setRadius(float radius)
    {
        PK_ASSERT(isValid(), "Trying to set radius of a CircleShape that is not yet created.", "Pekan");
        PK_ASSERT(radius > 0.0f, "CircleShape's radius must be greater than 0.", "Pekan");

        m_radius = radius;
        m_needUpdateVerticesLocal = true;
    }

    void CircleShape::setSegmentsCount(int segmentsCount)
    {
        PK_ASSERT(isValid(), "Trying to set segments count of a CircleShape that is not yet created.", "Pekan");
        PK_ASSERT(segmentsCount > 2, "CircleShape's segments must be at least 3.", "Pekan");

        m_segmentsCount = segmentsCount;
        m_needUpdateVerticesLocal = true;
        m_needUpdateIndices = true;
    }

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
    const Vertex2D* CircleShape::getVertices(float shapeIndex) const
#else
    const Vertex2D* CircleShape::getVertices() const
#endif
    {
        PK_ASSERT(isValid(), "Trying to get vertices of a CircleShape that is not yet created.", "Pekan");

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

        if (m_needUpdateVerticesLocal)
        {
            updateVerticesLocal();
        }
        if (m_needUpdateVerticesWorld)
        {
            updateVerticesWorld();
        }
        return m_verticesWorld.data();
    }

    const unsigned* CircleShape::getIndices() const
    {
        PK_ASSERT(isValid(), "Trying to get indices of a CircleShape that is not yet created.", "Pekan");

        if (m_needUpdateIndices)
        {
            updateIndices();
        }
        return m_indices.data();
    }

    void CircleShape::updateVerticesLocal() const
    {
        PK_ASSERT(isValid(), "Trying to update local vertices of a CircleShape that is not yet created.", "Pekan");

        m_verticesLocal.resize(m_segmentsCount);
        // Use radius and segments count to compute the local vertex positions
        for (int i = 0; i < m_segmentsCount; i++)
        {
            const float angle = float(i) * 2.0f * PI / m_segmentsCount;
            const float x = m_radius * cos(angle);
            const float y = m_radius * sin(angle);
            m_verticesLocal[i] = { x, y };
        }

        m_needUpdateVerticesLocal = false;
        m_needUpdateVerticesWorld = true;
    }

    void CircleShape::updateVerticesWorld() const
    {
        PK_ASSERT(isValid(), "Trying to update world vertices of a CircleShape that is not yet created.", "Pekan");

        const glm::mat3& worldMatrix = getWorldMatrix();
        m_verticesWorld.resize(m_verticesLocal.size());
        for (size_t i = 0; i < m_verticesLocal.size(); i++)
        {
            // Calculate world vertex positions by applying the transform matrix to the local vertex positions
            m_verticesWorld[i].position = glm::vec2(worldMatrix * glm::vec3(m_verticesLocal[i], 1.0f));
#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
            // Set "shapeIndex" attribute to be shape's index
            m_verticesWorld[i].shapeIndex = m_shapeIndex;
#else
            // Set "color" attribute to be shape's color
            m_verticesWorld[i].color = m_color;
#endif
        }

        // Cache change ID of the transform that we just used to update world vertices
        m_transformChangeIdUsedInVerticesWorld = Transformable2D::getChangeId();

        m_needUpdateVerticesWorld = false;
    }

    void CircleShape::updateIndices() const
    {
        PK_ASSERT(isValid(), "Trying to update indices of a CircleShape that is not yet created.", "Pekan");

        MathUtils::updateTriangleFanIndices(m_indices, m_segmentsCount);
        m_needUpdateIndices = false;
    }

} // namespace Renderer2D
} // namespace Renderer2D