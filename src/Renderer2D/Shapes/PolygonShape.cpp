#include "PolygonShape.h"

#include "PekanLogger.h"
#include "Utils/PekanUtils.h"
#include "Utils/MathUtils.h"
#include "RenderState.h"

#include <algorithm>

using namespace Pekan::Graphics;

namespace Pekan
{
namespace Renderer2D
{

    void PolygonShape::create(const std::vector<glm::vec2>& vertices)
    {
        Shape::_create();

        m_verticesLocal = vertices;
        m_isReversedVerticesLocal = false;
        m_isIndicesTriangleFan = false;
        m_isReversedIndices = false;
        m_needUpdateVerticesLocal = true;
    }

    void PolygonShape::setVertices(const std::vector<glm::vec2>& vertices)
    {
        PK_ASSERT(isValid(), "Trying to set vertices of a PolygonShape that is not yet created.", "Pekan");

        m_verticesLocal = vertices;
        m_isReversedVerticesLocal = false;
        m_isIndicesTriangleFan = false;
        m_isReversedIndices = false;
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

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
    const Vertex2D* PolygonShape::getVertices(float shapeIndex) const
#else
    const Vertex2D* PolygonShape::getVertices() const
#endif
    {
        PK_ASSERT(isValid(), "Trying to get vertices of a PolygonShape that is not yet created.", "Pekan");

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

    const unsigned* PolygonShape::getIndices() const
    {
        if (m_transformChangeIdUsedInVerticesWorld < Transformable2D::getChangeId())
        {
            m_needUpdateVerticesWorld = true;
        }

        // Indices are updated together with local vertices and world vertices,
        // so update local vertices and world vertices here if needed.
        if (m_needUpdateVerticesLocal)
        {
            updateVerticesLocal();
        }
        if (m_needUpdateVerticesWorld)
        {
            updateVerticesWorld();
        }

        PK_ASSERT_QUICK(m_indices.size() % 3 == 0);
        return m_indices.data();
    }

    void PolygonShape::updateVerticesLocal() const
    {
        PK_ASSERT(isValid(), "Trying to update local vertices of a PolygonShape that is not yet created.", "Pekan");

        // If given vertices form a non-convex polygon, we will need to triangulate it manually.
        if (!MathUtils::isPolygonConvex(m_verticesLocal))
        {
            // If given vertices form a CW polygon, reverse them, to ensure that we have a CCW polygon.
            // (This is needed to triangulate the polygon correctly)
            if (!MathUtils::isPolygonCCW(m_verticesLocal))
            {
                std::reverse(m_verticesLocal.begin(), m_verticesLocal.end());
                m_isReversedVerticesLocal = !m_isReversedVerticesLocal;
            }
            // Triangulate polygon,
            // filling the indices list with indices of triangles ready to be rendered.
            m_indices.clear();
            if (!MathUtils::triangulatePolygon(m_verticesLocal, m_indices))
            {
                PK_LOG_ERROR("Failed to triangulate a polygon. It's probably a badly defined polygon, possibly self-intersecting.", "Pekan");
            }
            m_isIndicesTriangleFan = false;
        }
        // Otherwise we can generate triangle fan indices.
        // (We can't use a triangle fan primitive because it would not work in ShapesBatch)
        else
        {

#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
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
                m_isReversedVerticesLocal = !m_isReversedVerticesLocal;
            }
#endif

            const size_t nVerts = m_verticesLocal.size();

            // If we already have some triangle fan indices and they are NOT reversed,
            // we just need to update them with the new number of vertices, extending/shortening the list as needed.
            if (m_isIndicesTriangleFan && !m_isReversedIndices)
            {
                MathUtils::updateTriangleFanIndices(m_indices, nVerts);
            }
            // Otherwise we don't have triangle fan indices, so we need to generate the whole list of indices anew.
            else
            {
                m_indices.resize((nVerts - 2) * 3);
                MathUtils::generateTriangleFanIndices(m_indices.data(), nVerts);
            }

            m_isIndicesTriangleFan = true;
        }

        m_isReversedIndices = false;

        m_needUpdateVerticesLocal = false;
        m_needUpdateVerticesWorld = true;
    }

    void PolygonShape::updateVerticesWorld() const
    {
        PK_ASSERT(isValid(), "Trying to update world vertices of a PolygonShape that is not yet created.", "Pekan");

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

#if PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING
        if (RenderState::isEnabledFaceCulling())
        {
            // If transform's orientation and indices orientation don't match
            if (MathUtils::isOrientationReversedByTransform(transformMatrix) != m_isReversedIndices)
            {
                // "Reverse" indices, flip every triangle
                for (size_t i = 0; i + 2 < m_indices.size(); i += 3)
                {
                    std::swap(m_indices[i + 1], m_indices[i + 2]);
                }
                m_isReversedIndices = !m_isReversedIndices;
            }
        }
#endif

        // Cache change ID of the transform that we just used to update world vertices
        m_transformChangeIdUsedInVerticesWorld = Transformable2D::getChangeId();

        m_needUpdateVerticesWorld = false;
    }

} // namespace Renderer2D
} // namespace Renderer2D