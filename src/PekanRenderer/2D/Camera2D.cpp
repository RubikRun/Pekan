#include "Camera2D.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Pekan
{
namespace Renderer
{

    Camera2D::Camera2D(float width, float height)
        : m_width(width)
        , m_height(height)
    {
        recalculateProjection();
        recalculateView();
    }

    void Camera2D::setPosition(const glm::vec2& position)
    {
        m_position = position;
        recalculateView();
    }

    void Camera2D::setZoom(float zoom)
    {
        m_zoom = zoom;
        recalculateProjection();
    }

    void Camera2D::recalculateProjection()
    {
        const float halfWidth = (m_width / 2.0f) / m_zoom;
        const float halfHeight = (m_height / 2.0f) / m_zoom;
        m_projectionMatrix = glm::ortho
        (
            -halfWidth, halfWidth,
            -halfHeight, halfHeight,
            -1.0f, 1.0f
        );

        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    }

    void Camera2D::recalculateView()
    {
        m_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-m_position, 0.0f));
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    }

    const glm::mat4& Camera2D::getViewProjectionMatrix() const
    {
        return m_viewProjectionMatrix;
    }

} // namespace Renderer
} // namespace Pekan