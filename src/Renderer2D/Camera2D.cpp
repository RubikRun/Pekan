#include "Camera2D.h"

#include "PekanLogger.h"
#include "PekanEngine.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Pekan
{
namespace Renderer2D
{

    Camera2D::~Camera2D()
    {
        if (m_isValid)
        {
            destroy();
        }
    }

    void Camera2D::create(float width, float height)
    {
        PK_ASSERT(!m_isValid, "Trying to create a Camera2D instance that is already created.", "Pekan");

        m_position = { 0.0f, 0.0f };
        m_zoom = 1.0f;
        m_viewMatrix = glm::mat4(1.0f);
        m_projectionMatrix = glm::mat4(1.0f);
        m_viewProjectionMatrix = glm::mat4(1.0f);

        m_isValid = true;

        setSize(width, height);
    }

    void Camera2D::create(float scale)
    {
        PK_ASSERT(!m_isValid, "Trying to create a Camera2D instance that is already created.", "Pekan");

        m_position = { 0.0f, 0.0f };
        m_zoom = 1.0f;
        m_viewMatrix = glm::mat4(1.0f);
        m_projectionMatrix = glm::mat4(1.0f);
        m_viewProjectionMatrix = glm::mat4(1.0f);

        m_isValid = true;

        setSize(scale);
    }

    void Camera2D::destroy()
    {
        PK_ASSERT(m_isValid, "Trying to destroy a Camera2D instance that is not yet created.", "Pekan");

        m_isValid = false;
    }

    void Camera2D::setSize(float width, float height)
    {
        PK_ASSERT(m_isValid, "Trying to set size of a Camera2D instance that is not yet created.", "Pekan");

        m_width = width;
        m_height = height;
        m_needUpdateViewMatrix = true;
        m_needUpdateProjectionMatrix = true;
    }

    void Camera2D::setSize(float scale)
    {
        PK_ASSERT(m_isValid, "Trying to set size of a Camera2D instance that is not yet created.", "Pekan");

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
        PK_ASSERT(m_isValid, "Trying to set position of a Camera2D instance that is not yet created.", "Pekan");

        m_position = position;
        m_needUpdateViewMatrix = true;
    }

    void Camera2D::move(glm::vec2 deltaPosition)
    {
        PK_ASSERT(m_isValid, "Trying to move a Camera2D instance that is not yet created.", "Pekan");

        m_position += deltaPosition;
        m_needUpdateViewMatrix = true;
    }

    void Camera2D::setZoom(float zoom)
    {
        PK_ASSERT(m_isValid, "Trying to set zoom level of a Camera2D instance that is not yet created.", "Pekan");

        m_zoom = zoom;
        m_needUpdateProjectionMatrix = true;
    }

    void Camera2D::zoomIn(float factor)
    {
        PK_ASSERT(m_isValid, "Trying to zoom in with a Camera2D instance that is not yet created.", "Pekan");

        m_zoom *= factor;
        m_needUpdateProjectionMatrix = true;
    }

    void Camera2D::zoomOut(float factor)
    {
        PK_ASSERT(m_isValid, "Trying to zoom out with a Camera2D instance that is not yet created.", "Pekan");

        m_zoom /= factor;
        m_needUpdateProjectionMatrix = true;
    }

    void Camera2D::updateViewMatrix() const
    {
        PK_ASSERT(m_isValid, "Trying to update view matrix of a Camera2D instance that is not yet created.", "Pekan");

        m_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-m_position, 0.0f));
    }

    void Camera2D::updateProjectionMatrix() const
    {
        PK_ASSERT(m_isValid, "Trying to update projection matrix of a Camera2D instance that is not yet created.", "Pekan");

        const float halfWidth = (m_width / 2.0f) / m_zoom;
        const float halfHeight = (m_height / 2.0f) / m_zoom;
        m_projectionMatrix = glm::ortho
        (
            -halfWidth, halfWidth,
            -halfHeight, halfHeight,
            -1.0f, 1.0f
        );
    }

    const glm::mat4& Camera2D::getViewProjectionMatrix() const
    {
        // Flag indicating if we need to update view projection matrix at the end of this function
        bool needUpdateViewProjectionMatrix = false;
        // Update view matrix if needed
        if (m_needUpdateViewMatrix)
        {
            updateViewMatrix();
            m_needUpdateViewMatrix = false;
            needUpdateViewProjectionMatrix = true;
        }
        // Update projection matrix if needed
        if (m_needUpdateProjectionMatrix)
        {
            updateProjectionMatrix();
            m_needUpdateProjectionMatrix = false;
            needUpdateViewProjectionMatrix = true;
        }
        // Update view projection matrix if needed, before returning it
        if (needUpdateViewProjectionMatrix)
        {
            m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        }
        return m_viewProjectionMatrix;
    }

    glm::vec2 Camera2D::windowToWorldPosition(glm::vec2 positionInWindow) const
    {
        PK_ASSERT(m_isValid, "Trying to convert a position from window space to world space with a Camera2D instance that is not yet created.", "Pekan");

        const glm::vec2 windowSize = glm::vec2(PekanEngine::getWindow().getSize());
        const glm::vec2 ndcPosition =
        {
            (positionInWindow.x / windowSize.x) * 2.0f - 1.0f,
            1.0f - (positionInWindow.y / windowSize.y) * 2.0f
        };
        return ndcToWorldPosition(ndcPosition);
    }

    glm::vec2 Camera2D::windowToWorldSize(glm::vec2 sizeInWindow) const
    {
        PK_ASSERT(m_isValid, "Trying to convert a size from window space to world space with a Camera2D instance that is not yet created.", "Pekan");

        const glm::vec2 windowSize = glm::vec2(PekanEngine::getWindow().getSize());
        const glm::vec2 sizeInNdc = sizeInWindow * 2.0f / windowSize;
        return ndcToWorldSize(sizeInNdc);
    }

    glm::vec2 Camera2D::worldToWindowPosition(glm::vec2 positionInWorld) const
    {
        PK_ASSERT(m_isValid, "Trying to convert a position from world space to window space with a Camera2D instance that is not yet created.", "Pekan");

        const glm::vec2 ndcPosition = worldToNdcPosition(positionInWorld);
        const glm::vec2 windowSize = glm::vec2(PekanEngine::getWindow().getSize());
        const glm::vec2 windowPosition = (ndcPosition + glm::vec2(1.0f, 1.0f)) * windowSize / 2.0f;
        return windowPosition;
    }

    glm::vec2 Camera2D::worldToWindowSize(glm::vec2 sizeInWorld) const
    {
        PK_ASSERT(m_isValid, "Trying to convert a size from world space to window space with a Camera2D instance that is not yet created.", "Pekan");

        const glm::vec2 ndcSize = worldToNdcSize(sizeInWorld);
        const glm::vec2 windowSize = glm::vec2(PekanEngine::getWindow().getSize());
        const glm::vec2 sizeInWindow = (ndcSize / 2.0f) * windowSize;
        return sizeInWindow;
    }

    glm::vec2 Camera2D::ndcToWorldPosition(glm::vec2 positionInNdc) const
    {
        PK_ASSERT(m_isValid, "Trying to convert a position from NDC space to world space with a Camera2D instance that is not yet created.", "Pekan");

        const glm::vec2 cameraSize = getSize();
        // Multiply NDC position by half camera's size, effectively scaling it to camera space,
        // then divide by zoom level to reverse the effect of the zoom,
        // then add camera's position to get the final position in world space.
        const glm::vec2 positionInWorld = (positionInNdc * (cameraSize * 0.5f)) / m_zoom + m_position;
        return positionInWorld;
    }

    glm::vec2 Camera2D::ndcToWorldSize(glm::vec2 sizeInNdc) const
    {
        PK_ASSERT(m_isValid, "Trying to convert a size from NDC space to world space with a Camera2D instance that is not yet created.", "Pekan");

        const glm::vec2 cameraSize = getSize();
        // Multiply NDC position by half camera's size, effectively scaling it to camera space,
        // then divide by zoom level to reverse the effect of the zoom,
        const glm::vec2 sizeInWorld = (sizeInNdc * (cameraSize * 0.5f)) / m_zoom;
        return sizeInWorld;
    }

    glm::vec2 Camera2D::worldToNdcPosition(glm::vec2 positionInWorld) const
    {
        PK_ASSERT(m_isValid, "Trying to convert a position from world space to NDC space with a Camera2D instance that is not yet created.", "Pekan");

        const glm::vec2 cameraSize = getSize();
        // Subtract camera position to bring into camera space, scale by zoom,
        // and then divide by half camera size to scale to NDC range
        const glm::vec2 ndcPos = ((positionInWorld - m_position) * m_zoom) / (cameraSize * 0.5f);
        return ndcPos;
    }

    glm::vec2 Camera2D::worldToNdcSize(glm::vec2 sizeInWorld) const
    {
        PK_ASSERT(m_isValid, "Trying to convert a size from world space to NDC space with a Camera2D instance that is not yet created.", "Pekan");

        const glm::vec2 cameraSize = getSize();
        // Scale by zoom and divide by half camera size
        const glm::vec2 sizeInNdc = (sizeInWorld * m_zoom) / (cameraSize * 0.5f);
        return sizeInNdc;
    }

} // namespace Renderer2D
} // namespace Pekan