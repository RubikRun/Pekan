#include <glm/gtc/constants.hpp>

namespace Pekan
{
namespace Renderer
{

    template <unsigned NSegments>
	void CircleShapeStatic<NSegments>::create
    (
        float radius,
        bool dynamic
    )
	{
        m_radius = radius;
        generateVertices();

        Shape::createRenderObject(dynamic);
	}

    template <unsigned NSegments>
    void CircleShapeStatic<NSegments>::destroy()
    {
        Shape::destroyRenderObject();
    }

    template <unsigned NSegments>
    void CircleShapeStatic<NSegments>::setRadius(float radius)
    {
        m_radius = radius;
        generateVertices();
        updateTransformedVertices();
    }

    template <unsigned NSegments>
    void CircleShapeStatic<NSegments>::updateTransformedVertices()
    {
        // Multiply local vertices by transform matrix to get world vertices.
        // NOTE: Local vertices are 2D, world vertices are also 2D,
        //       but the transform matrix is 3x3, so we need to convert a local vertex to 3D
        //       by adding a 3rd component of 1.0, then multiply it by the matrix, and then cut out the 3rd component,
        //       to get the final 2D world vertex.
        for (size_t i = 0; i < size_t(NSegments + 2); i++)
        {
            m_verticesWorld[i] = glm::vec2(m_transformMatrix * glm::vec3(m_verticesLocal[i], 1.0f));
        }

        Shape::updateRenderObject();
    }

    template <unsigned NSegments>
    void CircleShapeStatic<NSegments>::generateVertices()
    {
        m_verticesLocal[0] = glm::vec2(0.0f, 0.0f);

        for (int i = 1; i <= NSegments + 1; i++)
        {
            const float angle = i * 2.0f * glm::pi<float>() / NSegments;
            const float x = m_radius * cos(angle);
            const float y = m_radius * sin(angle);
            m_verticesLocal[i] = { x, y };
        }
    }

} // namespace Renderer
} // namespace Renderer