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
        // Calculate view matrix
        // by first translating the world in the opposite direction of camera's position,
        // then rotating in the opposite direction of camera's rotation.
        // (V = R^-1 * T^-1)
        const glm::mat4 viewMatrix = glm::translate
        (
            glm::rotate
            (
                glm::mat4(1.0f),
                -rotation,
                glm::vec3(0.0f, 0.0f, 1.0f)
            ),
            glm::vec3(-position, 0.0f)
        );

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

    void CameraComponent2D::rotate(float deltaRotation)
    {
        rotation += deltaRotation;
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

    glm::vec2 CameraComponent2D::windowToWorldVector(glm::vec2 vectorInWindow) const
    {
        ASSERT_CAMERA_IS_VALID;

        const glm::vec2 windowSize = glm::vec2(PekanEngine::getWindow().getSize());
        if (windowSize.x <= 0.0f && windowSize.y <= 0.0f)
        {
            PK_LOG_ERROR("Window size must be greater than 0 in each dimension.", "Pekan");
            return { 0.0f, 0.0f };
        }

        const glm::vec2 ndcVector = vectorInWindow * 2.0f / windowSize;
        return ndcToWorldVector(ndcVector);
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

    glm::vec2 CameraComponent2D::worldToWindowVector(glm::vec2 vectorInWorld) const
    {
        ASSERT_CAMERA_IS_VALID;

        const glm::vec2 windowSize = glm::vec2(PekanEngine::getWindow().getSize());
        PK_ASSERT(windowSize.x > 0.0f && windowSize.y > 0.0f, "Window size must be greater than 0 in each dimension.", "Pekan");

        const glm::vec2 ndcVector = worldToNdcVector(vectorInWorld);
        const glm::vec2 vectorInWindow = (ndcVector / 2.0f) * windowSize;
        return vectorInWindow;
    }

    glm::vec2 CameraComponent2D::ndcToWorldPosition(glm::vec2 positionInNdc) const
    {
        ASSERT_CAMERA_IS_VALID;
        if (zoomLevel <= 0.0f)
        {
            PK_LOG_ERROR("Camera's zoom level must be greater than 0.", "Pekan");
            return { 0.0f, 0.0f };
        }

        // Calculate position in camera space,
        // by multiplying NDC position by half camera's size,
        // then divide by zoom level to reverse the effect of the zoom.
        const glm::vec2 positionInCameraSpace = (positionInNdc * (size * 0.5f)) / zoomLevel;

        // Apply camera's rotation to get position in rotated camera space
        const float cosR = std::cos(rotation);
        const float sinR = std::sin(rotation);
        const glm::vec2 positionInRotatedCameraSpace =
        {
            positionInCameraSpace.x * cosR - positionInCameraSpace.y * sinR,
            positionInCameraSpace.x * sinR + positionInCameraSpace.y * cosR
        };

        // Add camera's position to get the final position in world space
        const glm::vec2 positionInWorld = positionInRotatedCameraSpace + position;

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

    glm::vec2 CameraComponent2D::ndcToWorldVector(glm::vec2 vectorInNdc) const
    {
        ASSERT_CAMERA_IS_VALID;
        if (zoomLevel <= 0.0f)
        {
            PK_LOG_ERROR("Camera's zoom level must be greater than 0.", "Pekan");
            return { 0.0f, 0.0f };
        }

        // Calculate vector in camera space,
        // by multiplying NDC vector by half camera's size,
        // then divide by zoom level to reverse the effect of the zoom.
        const glm::vec2 vectorInCameraSpace = (vectorInNdc * (size * 0.5f)) / zoomLevel;

        // Calculate vector in world space by applying camera's rotation
        const float cosR = std::cos(rotation);
        const float sinR = std::sin(rotation);
        const glm::vec2 vectorInWorld =
        {
            vectorInCameraSpace.x * cosR - vectorInCameraSpace.y * sinR,
            vectorInCameraSpace.x * sinR + vectorInCameraSpace.y * cosR
        };

        return vectorInWorld;
    }

    glm::vec2 CameraComponent2D::worldToNdcPosition(glm::vec2 positionInWorld) const
    {
        ASSERT_CAMERA_IS_VALID;
        if (size.x <= 0.0f || size.y <= 0.0f)
        {
            PK_LOG_ERROR("Camera's size must be greater than 0 in each dimension.", "Pekan");
            return { 0.0f, 0.0f };
        }

        // Calculate position in unrotated camera space by subtracting camera's position
        const glm::vec2 positionInUnrotatedCamera = positionInWorld - position;
        // Calculate position in camera space by applying the inverse of camera's rotation
        const float cosNegativeR = std::cos(-rotation);
        const float sinNegativeR = std::sin(-rotation);
        const glm::vec2 positionInCamera =
        {
            positionInUnrotatedCamera.x * cosNegativeR - positionInUnrotatedCamera.y * sinNegativeR,
            positionInUnrotatedCamera.x * sinNegativeR + positionInUnrotatedCamera.y * cosNegativeR
        };
        // Calculate position in NDC by applying camera's zoom and camera's size
        const glm::vec2 positionInNdc = positionInCamera * zoomLevel / (size * 0.5f);
        return positionInNdc;
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

    glm::vec2 CameraComponent2D::worldToNdcVector(glm::vec2 vectorInWorld) const
    {
        ASSERT_CAMERA_IS_VALID;
        if (size.x <= 0.0f || size.y <= 0.0f)
        {
            PK_LOG_ERROR("Camera's size must be greater than 0 in each dimension.", "Pekan");
            return { 0.0f, 0.0f };
        }

        // Calculate vector in camera space by applying the inverse of camera's rotation
        const float cosNegativeR = std::cos(-rotation);
        const float sinNegativeR = std::sin(-rotation);
        const glm::vec2 vectorInCamera =
        {
            vectorInWorld.x * cosNegativeR - vectorInWorld.y * sinNegativeR,
            vectorInWorld.x * sinNegativeR + vectorInWorld.y * cosNegativeR
        };
        // Calculate vector in NDC by applying camera's zoom and camera's size
        const glm::vec2 vectorInNdc = vectorInCamera * zoomLevel / (size * 0.5f);
        return vectorInNdc;
    }

} // namespace Renderer2D
}// namespace Pekan