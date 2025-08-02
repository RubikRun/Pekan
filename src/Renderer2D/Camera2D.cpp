#include "Camera2D.h"

#include "PekanEngine.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Pekan
{
namespace Renderer2D
{

    void Camera2D::setSize(float width, float height)
    {
        m_width = width;
        m_height = height;
        recalculateProjection();
        recalculateView();
    }

    void Camera2D::setSize(float scale)
    {
        const glm::ivec2 windowSize = PekanEngine::getWindow().getSize();
        if (windowSize.x > windowSize.y)
        {
            setSize(scale * float(windowSize.x) / float(windowSize.y), scale);
        }
        else
        {
            setSize(scale, scale * float(windowSize.y) / float(windowSize.x));
        }
    }

    void Camera2D::setPosition(glm::vec2 position)
    {
        m_position = position;
        recalculateView();
    }

    void Camera2D::move(glm::vec2 deltaPosition)
    {
        m_position += deltaPosition;
        recalculateView();
    }

    void Camera2D::setZoom(float zoom)
    {
        m_zoom = zoom;
        recalculateProjection();
    }

    void Camera2D::zoomIn(float factor)
    {
        m_zoom *= factor;
        recalculateProjection();
    }

    void Camera2D::zoomOut(float factor)
    {
        m_zoom /= factor;
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

    glm::vec2 Camera2D::windowToWorld(glm::vec2 windowPosition) const
    {
        const glm::vec2 windowSize = glm::vec2(PekanEngine::getWindow().getSize());

        // Convert window position to NDC
        const glm::vec2 ndcPosition =
        {
            (windowPosition.x / windowSize.x) * 2.0f - 1.0f,
            1.0f - (windowPosition.y / windowSize.y) * 2.0f
        };

        return ndcToWorld(ndcPosition);
    }

    glm::vec2 Camera2D::ndcToWorld(glm::vec2 ndcPosition) const
    {
        const glm::vec2 cameraSize = getSize();
        // Multiply NDC position by half camera's size, effectively scaling it to camera space,
        // then divide by zoom level to reverse the effect of the zoom,
        // then add camera's position to get the final position in world space.
        const glm::vec2 worldPos = (ndcPosition * (cameraSize * 0.5f)) / m_zoom + m_position;
        return worldPos;
    }

} // namespace Renderer2D
} // namespace Pekan