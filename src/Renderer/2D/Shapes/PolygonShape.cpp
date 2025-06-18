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
        m_vertices = vertices;

#ifndef NDEBUG
        if (!isConvex())
        {
            PK_LOG_ERROR("Class PolygonShape supports only convex polygons, but you are creating a non-convex one. "
                "It might not be rendered correctly", "Pekan");
        }
        if (PekanRenderer::isEnabledFaceCulling() && m_vertices.size() > 2
            && !isOrientationCCW(m_vertices[0], m_vertices[1], m_vertices[2]))
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
        m_vertices = vertices;

#ifndef NDEBUG
        if (!isConvex())
        {
            PK_LOG_ERROR("Class PolygonShape supports only convex polygons, but you have a non-convex one.  "
                "It might not be rendered correctly", "Pekan");
        }
        if (PekanRenderer::isEnabledFaceCulling() && m_vertices.size() > 2
            && !isOrientationCCW(m_vertices[0], m_vertices[1], m_vertices[2]))
        {
            PK_LOG_WARNING("Trying to set vertices of a PolygonShape resulting in CW (clockwise) orientation, but face culling is enabled in PekanRenderer,"
                " so your polygon will NOT be visible.", "Pekan");
        }
#endif

        _moveVertices(m_position);
    }

    void PolygonShape::setVertex(int index, glm::vec2 vertex)
    {
        if (index < 0 || index >= m_vertices.size())
        {
            PK_LOG_ERROR("Trying to set a vertex with an invalid index to a PolygonShape.", "Pekan");
            return;
        }
        m_vertices[index] = vertex + m_position;

#ifndef NDEBUG
        if (!isConvex())
        {
            PK_LOG_ERROR("Class PolygonShape supports only convex polygons, but you have a non-convex one.  "
                "It might not be rendered correctly", "Pekan");
        }
        if (PekanRenderer::isEnabledFaceCulling() && m_vertices.size() > 2
            && !isOrientationCCW(m_vertices[0], m_vertices[1], m_vertices[2]))
        {
            PK_LOG_WARNING("Trying to set a vertex of a PolygonShape resulting in CW (clockwise) orientation, but face culling is enabled in PekanRenderer,"
                " so your polygon will NOT be visible.", "Pekan");
        }
#endif

        Shape::updateRenderObject();
    }

    glm::vec2 PolygonShape::getVertex(int index) const
    {
        if (index < 0 || index >= m_vertices.size())
        {
            PK_LOG_ERROR("Trying to get a vertex with an invalid index from a PolygonShape.", "Pekan");
            return glm::vec2(0.0f, 0.0f);
        }
        return m_vertices[index] - m_position;
    }

    void PolygonShape::_moveVertices(glm::vec2 deltaPosition)
    {
        for (glm::vec2& vertex : m_vertices)
        {
            vertex += deltaPosition;
        }
        Shape::updateRenderObject();
    }

#ifndef NDEBUG
    bool PolygonShape::isConvex() const
    {
        if (m_vertices.size() < 3)
            return false;

        bool gotPositive = false;
        bool gotNegative = false;

        // Traverse polygon's vertices
        const int n = int(m_vertices.size());
        for (int i = 0; i < n; ++i)
        {
            // Reference the current 3 consecutive vertices A, B, C
            const glm::vec2& a = m_vertices[i];
            const glm::vec2& b = m_vertices[(i + 1) % n];
            const glm::vec2& c = m_vertices[(i + 2) % n];
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