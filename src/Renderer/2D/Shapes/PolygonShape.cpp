#include "PolygonShape.h"

#include "Utils/PekanUtils.h"

namespace Pekan
{
namespace Renderer
{

#ifndef NDEBUG
    // Checks if the orientation of 3 given vertices is CCW (counter-clockwise)
    static bool isOrientationCCW(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
    {
        const float det = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        return (det >= 0.0f);
    }
#endif

	void PolygonShape::create
    (
        const std::vector<glm::vec2>& vertices,
        bool dynamic
    )
	{
        m_verticesLocal = vertices;
        m_verticesWorld = m_verticesLocal;

#ifndef NDEBUG
        if (!isConvex())
        {
            PK_LOG_ERROR("Class PolygonShape supports only convex polygons, but you are creating a non-convex one. "
                "It might not be rendered correctly", "Pekan");
        }
        if (PekanRenderer::isEnabledFaceCulling() && m_verticesLocal.size() > 2
            && !isOrientationCCW(m_verticesLocal[0], m_verticesLocal[1], m_verticesLocal[2]))
        {
            PK_LOG_WARNING("Trying to create a PolygonShape with CW (clockwise) orientation, but face culling is enabled in PekanRenderer,"
                " so your polygon will NOT be visible.", "Pekan");
        }
#endif

        Shape::createRenderObject(dynamic);
	}

    void PolygonShape::destroy()
    {
        Shape::destroyRenderObject();
    }

    void PolygonShape::setVertices(const std::vector<glm::vec2>& vertices)
    {
        m_verticesLocal = vertices;

#ifndef NDEBUG
        if (!isConvex())
        {
            PK_LOG_ERROR("Class PolygonShape supports only convex polygons, but you have a non-convex one.  "
                "It might not be rendered correctly", "Pekan");
        }
        if (PekanRenderer::isEnabledFaceCulling() && m_verticesLocal.size() > 2
            && !isOrientationCCW(m_verticesLocal[0], m_verticesLocal[1], m_verticesLocal[2]))
        {
            PK_LOG_WARNING("Trying to set vertices of a PolygonShape resulting in CW (clockwise) orientation, but face culling is enabled in PekanRenderer,"
                " so your polygon will NOT be visible.", "Pekan");
        }
#endif

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

#ifndef NDEBUG
        if (!isConvex())
        {
            PK_LOG_ERROR("Class PolygonShape supports only convex polygons, but you have a non-convex one.  "
                "It might not be rendered correctly", "Pekan");
        }
        if (PekanRenderer::isEnabledFaceCulling() && m_verticesLocal.size() > 2
            && !isOrientationCCW(m_verticesLocal[0], m_verticesLocal[1], m_verticesLocal[2]))
        {
            PK_LOG_WARNING("Trying to set a vertex of a PolygonShape resulting in CW (clockwise) orientation, but face culling is enabled in PekanRenderer,"
                " so your polygon will NOT be visible.", "Pekan");
        }
#endif

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

#ifndef NDEBUG
    bool PolygonShape::isConvex() const
    {
        if (m_verticesLocal.size() < 3)
            return false;

        bool gotPositive = false;
        bool gotNegative = false;

        // Traverse polygon's vertices
        const int n = int(m_verticesLocal.size());
        for (int i = 0; i < n; ++i)
        {
            // Reference the current 3 consecutive vertices A, B, C
            const glm::vec2& a = m_verticesLocal[i];
            const glm::vec2& b = m_verticesLocal[(i + 1) % n];
            const glm::vec2& c = m_verticesLocal[(i + 2) % n];
            // Calculate the vectors A->B and B->C
            const glm::vec2 ab = b - a;
            const glm::vec2 bc = c - b;
            // Calculate cross product of A->B and B->C
            const float cross = ab.x * bc.y - ab.y * bc.x;

            if (cross > 0) gotPositive = true;
            else if (cross < 0) gotNegative = true;

            // If there are both positive and negative cross products,
            // this means that the polygon's sides sometimes turn left and sometimes right,
            // which means that it's concave, hence not convex
            if (gotPositive && gotNegative)
            {
                return false;
            }
        }

        // At this point all cross products have been either positive or negative,
        // so all sides are turning either to the left or to the right,
        // which means that the polygon is convex
        return true;
    }
#endif

} // namespace Renderer
} // namespace Renderer