#include "LineShape.h"

#include "Utils/PekanUtils.h"

namespace Pekan
{
namespace Renderer
{

    // Indices of vertices of the 2 triangles making up the line
    static const unsigned INDICES[6] = { 0, 1, 2, 0, 2, 3 };

	void LineShape::create
    (
        glm::vec2 pointA, glm::vec2 pointB, float thickness,
        bool dynamic
    )
	{
        Shape::create();

        m_pointA = pointA;
        m_pointB = pointB;
        m_thickness = thickness;

        generateVertices();

        Shape::createRenderObject(m_vertices, INDICES, dynamic);
	}

    void LineShape::setPointA(glm::vec2 pointA)
    {
        m_pointA = pointA;
        generateVertices();
        updateRenderObject();
    }

    void LineShape::setPointB(glm::vec2 pointB)
    {
        m_pointB = pointB;
        generateVertices();
        updateRenderObject();
    }

    void LineShape::setThickness(float thickness)
    {
        m_thickness = thickness;
        generateVertices();
        updateRenderObject();
    }

    void LineShape::_moveVertices(glm::vec2 deltaPosition)
    {
        m_vertices[0] += deltaPosition;
        m_vertices[1] += deltaPosition;
        m_vertices[2] += deltaPosition;
        m_vertices[3] += deltaPosition;
        updateRenderObject();
    }

    void LineShape::generateVertices()
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
        m_vertices[0] = m_pointA + normalOffset;
        m_vertices[1] = m_pointA - normalOffset;
        m_vertices[2] = m_pointB - normalOffset;
        m_vertices[3] = m_pointB + normalOffset;
    }

} // namespace Renderer
} // namespace Renderer