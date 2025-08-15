#include "CircleShapeStatic.h"

#include "PekanLogger.h"
#include "Utils/MathUtils.h"

#include <glm/gtc/constants.hpp>

namespace Pekan
{
namespace Renderer2D
{

    template <unsigned NSegments>
    void CircleShapeStatic<NSegments>::create(float radius)
    {
        PK_ASSERT(radius > 0.0f, "CircleShapeStatic's radius must be greater than 0.", "Pekan");

        Shape::_create();

        m_radius = radius;
        m_needUpdateVerticesLocal = true;
    }

    template <unsigned NSegments>
    void CircleShapeStatic<NSegments>::setRadius(float radius)
    {
        PK_ASSERT(isValid(), "Trying to set radius of a CircleShapeStatic that is not yet created.", "Pekan");
        PK_ASSERT(radius > 0.0f, "CircleShapeStatic's radius must be greater than 0.", "Pekan");

        m_radius = radius;
        m_needUpdateVerticesLocal = true;
    }

    template<unsigned NSegments>
#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
    const Vertex2D* CircleShapeStatic<NSegments>::getVertices(float shapeIndex) const
#else
    const Vertex2D* CircleShapeStatic<NSegments>::getVertices() const
#endif
    {
        PK_ASSERT(isValid(), "Trying to get vertices of a CircleShapeStatic that is not yet created.", "Pekan");

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

    template <unsigned NSegments>
    void CircleShapeStatic<NSegments>::updateVerticesLocal() const
    {
        PK_ASSERT(isValid(), "Trying to update local vertices of a CircleShape that is not yet created.", "Pekan");

        for (int i = 0; i < NSegments; i++)
        {
            const float angle = i * 2.0f * glm::pi<float>() / NSegments;
            const float x = m_radius * cos(angle);
            const float y = m_radius * sin(angle);
            m_verticesLocal[i] = { x, y };
        }

        m_needUpdateVerticesLocal = false;
        m_needUpdateVerticesWorld = true;
    }

    template <unsigned NSegments>
    void CircleShapeStatic<NSegments>::updateVerticesWorld() const
    {
        PK_ASSERT(isValid(), "Trying to update world vertices of a CircleShape that is not yet created.", "Pekan");

        const glm::mat3& transformMatrix = getTransformMatrix();
        for (size_t i = 0; i < size_t(NSegments); i++)
        {
            // Calculate world vertex positions by applying the transform matrix to the local vertex positions
            m_verticesWorld[i].position = glm::vec2(transformMatrix * glm::vec3(m_verticesLocal[i], 1.0f));

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
            // Set "shapeIndex" attribute to be shape's index
            m_verticesWorld[i].shapeIndex = m_shapeIndex;
#else
            // Set "color" attribute to be shape's color
            m_verticesWorld[i].color = m_color;
#endif
        }

        m_needUpdateVerticesWorld = false;
    }

    template<unsigned NSegments>
    void CircleShapeStatic<NSegments>::generateIndices()
    {
        MathUtils::generateTriangleFanIndices(m_indices, NSegments);
    }

} // namespace Renderer2D
} // namespace Renderer2D