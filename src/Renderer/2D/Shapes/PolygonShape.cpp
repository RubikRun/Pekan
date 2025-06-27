#include "PolygonShape.h"

#include "Utils/PekanUtils.h"
#include "Utils/MathUtils.h"

namespace Pekan
{
namespace Renderer
{

    void PolygonShape::create(const std::vector<glm::vec2>& vertices)
    {
        Shape::create();

        m_verticesLocal = vertices;
        m_isReversedVerticesLocal = false;
        m_needUpdateVerticesLocal = true;
    }

    void PolygonShape::setVertices(const std::vector<glm::vec2>& vertices)
    {
        PK_ASSERT(isValid(), "Trying to set vertices of a PolygonShape that is not yet created.", "Pekan");

        m_verticesLocal = vertices;
        m_isReversedVerticesLocal = false;
        m_needUpdateVerticesLocal = true;
    }

    void PolygonShape::setVertex(int index, glm::vec2 vertex)
    {
        PK_ASSERT(isValid(), "Trying to set vertex of a PolygonShape that is not yet created.", "Pekan");
        if (index < 0 || index >= int(m_verticesLocal.size()))
        {
            PK_LOG_ERROR("Trying to set a vertex out of bounds to a PolygonShape.", "Pekan");
            return;
        }

        if (m_isReversedVerticesLocal)
        {
            m_verticesLocal[m_verticesLocal.size() - 1 - index] = vertex;
        }
        else
        {
            m_verticesLocal[index] = vertex;
        }

        m_needUpdateVerticesLocal = true;
    }

    const ShapeVertex* PolygonShape::getVertices() const
    {
        PK_ASSERT(isValid(), "Trying to get vertices of a PolygonShape that is not yet created.", "Pekan");

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

    void PolygonShape::updateVerticesLocal() const
    {
        m_isReversedVerticesLocal = false;

        // If given vertices form a non-convex polygon, we will need to triangulate it manually.
        if (!MathUtils::isPolygonConvex(m_verticesLocal))
        {
            // If given vertices form a CW polygon, reverse them, to ensure that we have a CCW polygon.
            if (!MathUtils::isPolygonCCW(m_verticesLocal))
            {
                std::reverse(m_verticesLocal.begin(), m_verticesLocal.end());
                m_isReversedVerticesLocal = true;
            }
            // Triangulate polygon,
            // filling the indices list with indices of triangles ready to be rendered.
            m_indices.clear();
            if (!MathUtils::triangulatePolygon(m_verticesLocal, m_indices))
            {
                PK_LOG_ERROR("Failed to triangulate a polygon. It's probably a badly defined polygon, possibly self-intersecting.", "Pekan");
            }
        }
        // Otherwise we can leave the indices empty, and then a triangle fan primitive will be used,
        // which automatically triangulates the polygon.
        else
        {
            m_indices.clear();

            // If given vertices form a CW polygon, reverse them, to ensure that we have a CCW polygon,
            // but only if face culling is enabled - otherwise CW polygons are ok.
            if
            (
                RenderState::isEnabledFaceCulling()
                && m_verticesLocal.size() > 2
                // We already know that the polygon is convex here, so checking if it's CW can be done
                // by just checking the first 3 vertices, no need to waste performance on the rest.
                && !MathUtils::isOrientationCCW(m_verticesLocal[0], m_verticesLocal[1], m_verticesLocal[2])
            )
            {
                std::reverse(m_verticesLocal.begin(), m_verticesLocal.end());
                m_isReversedVerticesLocal = true;
            }
        }

        m_needUpdateVerticesLocal = false;
        m_needUpdateVerticesWorld = true;
    }

    void PolygonShape::updateVerticesWorld() const
    {
        PK_ASSERT(isValid(), "Trying to update world vertices of a PolygonShape that is not yet created.", "Pekan");

        const glm::mat3& transformMatrix = getTransformMatrix();
        m_verticesWorld.resize(m_verticesLocal.size());
        for (size_t i = 0; i < m_verticesLocal.size(); i++)
        {
            // Calculate world vertex positions by applying the transform matrix to the local vertex positions
            m_verticesWorld[i].position = glm::vec2(transformMatrix * glm::vec3(m_verticesLocal[i], 1.0f));
            // Set vertex colors equal to shape's color
            m_verticesWorld[i].color = m_color;
        }

        m_needUpdateVerticesWorld = false;
    }

} // namespace Renderer
} // namespace Renderer