#include "LineShape.h"

#include "Utils/PekanUtils.h"

namespace Pekan
{
namespace Renderer
{

    const unsigned LineShape::s_indices[6] = { 0, 1, 2, 0, 2, 3 };

	void LineShape::create
    (
        glm::vec2 pointA, glm::vec2 pointB, float thickness,
        bool dynamic
    )
	{
        m_pointA = pointA;
        m_pointB = pointB;
        m_thickness = thickness;

        generateVerticesLocal();
        m_verticesWorld[0] = m_verticesLocal[0];
        m_verticesWorld[1] = m_verticesLocal[1];
        m_verticesWorld[2] = m_verticesLocal[2];
        m_verticesWorld[3] = m_verticesLocal[3];

        Shape::createRenderObject(dynamic);
	}

    void LineShape::destroy()
    {
        Shape::destroyRenderObject();
    }

    void LineShape::setPointA(glm::vec2 pointA)
    {
        m_pointA = pointA;
        generateVerticesLocal();
        updateTransformedVertices();
    }

    void LineShape::setPointB(glm::vec2 pointB)
    {
        m_pointB = pointB;
        generateVerticesLocal();
        updateTransformedVertices();
    }

    void LineShape::setThickness(float thickness)
    {
        m_thickness = thickness;
        generateVerticesLocal();
        updateTransformedVertices();
    }

    void LineShape::updateTransformedVertices()
    {
        // Multiply local vertices by transform matrix to get world vertices.
        // NOTE: Local vertices are 2D, world vertices are also 2D,
        //       but the transform matrix is 3x3, so we need to convert a local vertex to 3D
        //       by adding a 3rd component of 1.0, then multiply it by the matrix, and then cut out the 3rd component,
        //       to get the final 2D world vertex.
        m_verticesWorld[0] = glm::vec2(m_transformMatrix * glm::vec3(m_verticesLocal[0], 1.0f));
        m_verticesWorld[1] = glm::vec2(m_transformMatrix * glm::vec3(m_verticesLocal[1], 1.0f));
        m_verticesWorld[2] = glm::vec2(m_transformMatrix * glm::vec3(m_verticesLocal[2], 1.0f));
        m_verticesWorld[3] = glm::vec2(m_transformMatrix * glm::vec3(m_verticesLocal[3], 1.0f));

        Shape::updateRenderObject();
    }

    void LineShape::generateVerticesLocal()
    {
        // Calculate the normal offset vector.
        // We'll use that vector to "thicken" the line to the required thickness.
        // 
        // The ideal line AB has 0 thickness.
        // To make it have thickness T we need to turn the 2 points A and B into 4 points.
        // Point A will turn into 2 points, and point B will turn into 2 points.
        // The way to turn an original point into 2 points is to add a vector of length T / 2 in the 2 directions perpendicular to AB.
        // The normal offset vector is just that - a vector in one of the perpendicular directions with length T / 2.
        // The vector in the other perpendicular direction will be just the negative of that vector
        const glm::vec2 dir = glm::normalize(m_pointB - m_pointA);
        const glm::vec2 normalOffset
        (
            -(m_thickness / 2.0f) * dir.y,
            (m_thickness / 2.0f) * dir.x
        );
        // To get the 4 vertices of the line we can add and subtract the normal offset vector to A and B.
        m_verticesLocal[0] = m_pointA + normalOffset;
        m_verticesLocal[1] = m_pointA - normalOffset;
        m_verticesLocal[2] = m_pointB - normalOffset;
        m_verticesLocal[3] = m_pointB + normalOffset;
    }

} // namespace Renderer
} // namespace Renderer