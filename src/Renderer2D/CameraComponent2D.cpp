#include "CameraComponent2D.h"

#include "PekanEngine.h"
#include "PekanLogger.h"

#include <glm/gtc/matrix_transform.hpp>

// A macro to assert that a camera's fields are valid
#define ASSERT_CAMERA_IS_VALID \
    PK_ASSERT(size.x > 0.0f && size.y > 0.0f, "Camera's size must be greater than 0 in each dimension.", "Pekan"); \
    PK_ASSERT(zoomLevel > 0.0f, "Camera's zoom level must be greater than 0.", "Pekan");

namespace Pekan
{
namespace Renderer2D
{

    glm::mat4 CameraComponent2D::getViewProjectionMatrix() const
    {
        ASSERT_CAMERA_IS_VALID;

        const float halfWidth = (size.x / 2.0f) / zoomLevel;
        const float halfHeight = (size.y / 2.0f) / zoomLevel;
        const glm::mat4 projectionMatrix = glm::ortho
        (
            -halfWidth, halfWidth,
            -halfHeight, halfHeight,
            -1.0f, 1.0f
        );
        const glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-position, 0.0f));
        const glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
        return viewProjectionMatrix;
    }

    void CameraComponent2D::setWidth(float width, bool lockToWindowAspecRatio)
    {
        if (width <= 0.0f)
        {
            PK_LOG_ERROR("Camera's width must be greater than 0.", "Pekan");
            return;
        }

        size.x = width;
        // If we need to lock camera's aspect ratio to window's aspect ratio, adjust height accordingly
        if (lockToWindowAspecRatio)
        {
            const float windowAspectRatio = PekanEngine::getWindow().getAspectRatio();
            PK_ASSERT(windowAspectRatio > 0.0f, "Window aspect ratio must be greater than 0.", "Pekan");
            size.y = width / windowAspectRatio;
        }
    }

    void CameraComponent2D::setHeight(float height, bool lockToWindowAspecRatio)
    {
        if (height <= 0.0f)
        {
            PK_LOG_ERROR("Camera's height must be greater than 0.", "Pekan");
            return;
        }

        size.y = height;
        // If we need to lock camera's aspect ratio to window's aspect ratio, adjust width accordingly
        if (lockToWindowAspecRatio)
        {
            const float windowAspectRatio = PekanEngine::getWindow().getAspectRatio();
            PK_ASSERT(windowAspectRatio > 0.0f, "Window aspect ratio must be greater than 0.", "Pekan");
            size.x = height * windowAspectRatio;
        }
    }

    void CameraComponent2D::move(glm::vec2 deltaPosition)
    {
        position += deltaPosition;
    }

    void CameraComponent2D::zoomIn(float factor)
    {
        if (factor <= 0.0f)
        {
            PK_LOG_ERROR("Zoom in factor must be greater than 0.", "Pekan");
            return;
        }

        zoomLevel *= factor;
    }

    void CameraComponent2D::zoomOut(float factor)
    {
        if (factor <= 0.0f)
        {
            PK_LOG_ERROR("Zoom out factor must be greater than 0.", "Pekan");
            return;
        }

        zoomLevel /= factor;
    }

    glm::vec2 CameraComponent2D::windowToWorldPosition(glm::vec2 positionInWindow) const
    {
        ASSERT_CAMERA_IS_VALID;

        const glm::vec2 windowSize = glm::vec2(PekanEngine::getWindow().getSize());
        if (windowSize.x <= 0.0f && windowSize.y <= 0.0f)
        {
            PK_LOG_ERROR("Window size must be greater than 0 in each dimension.", "Pekan");
            return { 0.0f, 0.0f };
        }

        const glm::vec2 ndcPosition =
        {
            (positionInWindow.x / windowSize.x) * 2.0f - 1.0f,
            1.0f - (positionInWindow.y / windowSize.y) * 2.0f
        };
        return ndcToWorldPosition(ndcPosition);
    }

    glm::vec2 CameraComponent2D::windowToWorldSize(glm::vec2 sizeInWindow) const
    {
        ASSERT_CAMERA_IS_VALID;

        const glm::vec2 windowSize = glm::vec2(PekanEngine::getWindow().getSize());
        if (windowSize.x <= 0.0f && windowSize.y <= 0.0f)
        {
            PK_LOG_ERROR("Window size must be greater than 0 in each dimension.", "Pekan");
            return { 0.0f, 0.0f };
        }

        const glm::vec2 sizeInNdc = sizeInWindow * 2.0f / windowSize;
        return ndcToWorldSize(sizeInNdc);
    }

    glm::vec2 CameraComponent2D::worldToWindowPosition(glm::vec2 positionInWorld) const
    {
        ASSERT_CAMERA_IS_VALID;

        const glm::vec2 windowSize = glm::vec2(PekanEngine::getWindow().getSize());
        PK_ASSERT(windowSize.x > 0.0f && windowSize.y > 0.0f, "Window size must be greater than 0 in each dimension.", "Pekan");

        const glm::vec2 ndcPosition = worldToNdcPosition(positionInWorld);
        const glm::vec2 windowPosition = (ndcPosition + glm::vec2(1.0f, 1.0f)) * windowSize / 2.0f;
        return windowPosition;
    }

    glm::vec2 CameraComponent2D::worldToWindowSize(glm::vec2 sizeInWorld) const
    {
        ASSERT_CAMERA_IS_VALID;

        const glm::vec2 windowSize = glm::vec2(PekanEngine::getWindow().getSize());
        PK_ASSERT(windowSize.x > 0.0f && windowSize.y > 0.0f, "Window size must be greater than 0 in each dimension.", "Pekan");

        const glm::vec2 ndcSize = worldToNdcSize(sizeInWorld);
        const glm::vec2 sizeInWindow = (ndcSize / 2.0f) * windowSize;
        return sizeInWindow;
    }

    glm::vec2 CameraComponent2D::ndcToWorldPosition(glm::vec2 positionInNdc) const
    {
        ASSERT_CAMERA_IS_VALID;
        if (zoomLevel <= 0.0f)
        {
            PK_LOG_ERROR("Camera's zoom level must be greater than 0.", "Pekan");
            return { 0.0f, 0.0f };
        }

        // Multiply NDC position by half camera's size, effectively scaling it to camera space,
        // then divide by zoom level to reverse the effect of the zoom,
        // then add camera's position to get the final position in world space.
        const glm::vec2 positionInWorld = (positionInNdc * (size * 0.5f)) / zoomLevel + position;
        return positionInWorld;
    }

    glm::vec2 CameraComponent2D::ndcToWorldSize(glm::vec2 sizeInNdc) const
    {
        ASSERT_CAMERA_IS_VALID;
        if (zoomLevel <= 0.0f)
        {
            PK_LOG_ERROR("Camera's zoom level must be greater than 0.", "Pekan");
            return { 0.0f, 0.0f };
        }

        // Multiply NDC position by half camera's size, effectively scaling it to camera space,
        // then divide by zoom level to reverse the effect of the zoom,
        const glm::vec2 sizeInWorld = (sizeInNdc * (size * 0.5f)) / zoomLevel;
        return sizeInWorld;
    }

    glm::vec2 CameraComponent2D::worldToNdcPosition(glm::vec2 positionInWorld) const
    {
        ASSERT_CAMERA_IS_VALID;
        if (size.x <= 0.0f || size.y <= 0.0f)
        {
            PK_LOG_ERROR("Camera's size must be greater than 0 in each dimension.", "Pekan");
            return { 0.0f, 0.0f };
        }

        // Subtract camera position to bring into camera space, scale by zoom,
        // and then divide by half camera size to scale to NDC range
        const glm::vec2 ndcPos = ((positionInWorld - position) * zoomLevel) / (size * 0.5f);
        return ndcPos;
    }

    glm::vec2 CameraComponent2D::worldToNdcSize(glm::vec2 sizeInWorld) const
    {
        ASSERT_CAMERA_IS_VALID;
        if (size.x <= 0.0f || size.y <= 0.0f)
        {
            PK_LOG_ERROR("Camera's size must be greater than 0 in each dimension.", "Pekan");
            return { 0.0f, 0.0f };
        }

        // Scale by zoom and divide by half camera size
        const glm::vec2 sizeInNdc = (sizeInWorld * zoomLevel) / (size * 0.5f);
        return sizeInNdc;
    }

    float CameraComponent2D::getLeftEdgeInWorldSpace() const
    {
        ASSERT_CAMERA_IS_VALID;
        if (zoomLevel <= 0.0f)
        {
            PK_LOG_ERROR("Camera's zoom level must be greater than 0.", "Pekan");
            return 0.0f;
        }

        return position.x - (size.x / 2.0f) / zoomLevel;
    }

    float CameraComponent2D::getRightEdgeInWorldSpace() const
    {
        ASSERT_CAMERA_IS_VALID;
        if (zoomLevel <= 0.0f)
        {
            PK_LOG_ERROR("Camera's zoom level must be greater than 0.", "Pekan");
            return 0.0f;
        }

        return position.x + (size.x / 2.0f) / zoomLevel;
    }

    float CameraComponent2D::getTopEdgeInWorldSpace() const
    {
        ASSERT_CAMERA_IS_VALID;
        if (zoomLevel <= 0.0f)
        {
            PK_LOG_ERROR("Camera's zoom level must be greater than 0.", "Pekan");
            return 0.0f;
        }

        return position.y + (size.y / 2.0f) / zoomLevel;
    }

    float CameraComponent2D::getBottomEdgeInWorldSpace() const
    {
        ASSERT_CAMERA_IS_VALID;
        if (zoomLevel <= 0.0f)
        {
            PK_LOG_ERROR("Camera's zoom level must be greater than 0.", "Pekan");
            return 0.0f;
        }

        return position.y - (size.y / 2.0f) / zoomLevel;
    }

} // namespace Renderer2D
}// namespace Pekan