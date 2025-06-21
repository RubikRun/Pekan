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

        Shape::createRenderObject(dynamic);
	}

    void PolygonShape::destroy()
    {
        Shape::destroyRenderObject();
    }

    void PolygonShape::setVertices(const std::vector<glm::vec2>& vertices)
    {
        m_verticesLocal = vertices;

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

    bool PolygonShape::isConvex() const
    {
        if (m_verticesLocal.size() < 3)
            return false;

        bool foundCW = false;
        bool foundCCW = false;

        // Traverse polygon's vertices
        const int n = int(m_verticesLocal.size());
        for (int i = 0; i < n; ++i)
        {
            // Reference the current 3 consecutive vertices A, B, C
            const glm::vec2& a = m_verticesLocal[i];
            const glm::vec2& b = m_verticesLocal[(i + 1) % n];
            const glm::vec2& c = m_verticesLocal[(i + 2) % n];
            // Compute determinant of ABC
            const float detABC = MathUtils::getDeterminant(a, b, c);

            if (detABC > 0) foundCW = true;
            else if (detABC < 0) foundCCW = true;

            // If there are both positive and negative determinants,
            // this means that the polygon's sides sometimes turn left and sometimes right,
            // which means that it's concave, hence not convex.
            if (foundCW && foundCCW)
            {
                return false;
            }
        }

        // At this point all determinants have been either positive or negative,
        // so all sides are turning either to the left or to the right,
        // which means that the polygon is convex
        return true;
    }

} // namespace Renderer
} // namespace Renderer