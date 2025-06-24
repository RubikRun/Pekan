#include "PolygonShape.h"

#include "Utils/PekanUtils.h"
#include "Utils/MathUtils.h"

namespace Pekan
{
namespace Renderer
{

	void PolygonShape::create
    (
        const std::vector<glm::vec2>& vertices,
        bool dynamic
    )
    {
        m_verticesLocal = vertices;
        m_verticesWorld = m_verticesLocal;

        handleNewVerticesLocal();
        Shape::createRenderObject(dynamic);
    }

    void PolygonShape::destroy()
    {
        Shape::destroyRenderObject();
    }

    void PolygonShape::setVertices(const std::vector<glm::vec2>& vertices)
    {
        m_verticesLocal = vertices;
        m_verticesWorld.resize(m_verticesLocal.size());

        handleNewVerticesLocal();
        updateTransformedVertices();
    }

    void PolygonShape::setVertex(int index, glm::vec2 vertex)
    {
        if (index < 0 || index >= m_verticesLocal.size())
        {
            PK_LOG_ERROR("Trying to set a vertex with an invalid index to a PolygonShape.", "Pekan");
            return;
        }
        m_verticesLocal[index] = vertex;

        handleNewVerticesLocal();
        updateTransformedVertices();
    }

    glm::vec2 PolygonShape::getVertex(int index) const
    {
        if (index < 0 || index >= m_verticesLocal.size())
        {
            PK_LOG_ERROR("Trying to get a vertex with an invalid index from a PolygonShape.", "Pekan");
            return glm::vec2(0.0f, 0.0f);
        }
        return m_verticesLocal[index];
    }

    void PolygonShape::updateTransformedVertices()
    {
        // Multiply local vertices by transform matrix to get world vertices.
        // NOTE: Local vertices are 2D, world vertices are also 2D,
        //       but the transform matrix is 3x3, so we need to convert a local vertex to 3D
        //       by adding a 3rd component of 1.0, then multiply it by the matrix, and then cut out the 3rd component,
        //       to get the final 2D world vertex.
        for (size_t i = 0; i < m_verticesLocal.size(); i++)
        {
            m_verticesWorld[i] = glm::vec2(m_transformMatrix * glm::vec3(m_verticesLocal[i], 1.0f));
        }

        Shape::updateRenderObject();
    }

    void PolygonShape::triangulate()
    {
        m_indices.clear();
        if (!MathUtils::triangulatePolygon(m_verticesLocal, m_indices))
        {
            PK_LOG_ERROR("Failed to triangulate a polygon. It's probably a badly defined polygon, possibly self-intersecting.", "Pekan");
        }
    }

    void PolygonShape::handleNewVerticesLocal()
    {
        // If given vertices form a non-convex polygon, we will need to triangulate it manually.
        if (!MathUtils::isPolygonConvex(m_verticesLocal))
        {
            // If given vertices form a CW polygon, reverse them, to ensure that we have a CCW polygon.
            if (!MathUtils::isPolygonCCW(m_verticesLocal))
            {
                std::reverse(m_verticesLocal.begin(), m_verticesLocal.end());
            }
            triangulate();
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
            }
        }
    }

} // namespace Renderer
} // namespace Renderer