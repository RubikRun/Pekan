#pragma once

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

    struct CameraComponent2D
    {
    ///// Data /////

        // Size of camera's view area, in world space
        glm::vec2 size = { 0.0f, 0.0f };
        // Position of the center of camera's view area, in world space
        glm::vec2 position = { 0.0f, 0.0f };
        // Camera's zoom level.
        // Greater than 1.0 means zoomed in, less than 1.0 means zoomed out
        float zoomLevel = 1.0f;

        // A flag indicating if this camera is the primary camera in the scene.
        // The "primary" camera is the one used for rendering.
        // Only one camera in a scene can be the primary camera.
        bool isPrimary = true;
        // A flag indicating if this camera can be controlled by user input,
        // panning and zooming with the mouse.
        // Only one camera in a scene can be the controllable camera.
        bool isControllable = true;

    ////////////////


    /* functions */

        // Returns camera's view projection matrix
        glm::mat4 getViewProjectionMatrix() const;

        // Sets camera's width
        // @param[in] lockToWindowAspecRatio - If true, height is adjusted to match window's aspect ratio
        void setWidth(float width, bool lockToWindowAspecRatio = true);
        // Sets camera's height
        // @param[in] lockToWindowAspecRatio - If true, width is adjusted to match window's aspect ratio
        void setHeight(float height, bool lockToWindowAspecRatio = true);

        // Moves camera by some amount, in world space
        void move(glm::vec2 deltaPosition);

        // Zooms in with the camera, multiplying the current zoom level by some factor
        void zoomIn(float factor);
        // Zooms out with the camera, dividing the current zoom level by some factor
        void zoomOut(float factor);

        // Converts a given position/size from window space to world space, using the camera.
        glm::vec2 windowToWorldPosition(glm::vec2 positionInWindow) const;
        glm::vec2 windowToWorldSize(glm::vec2 sizeInWindow) const;
        // Converts a given position/size from world space to window space, using the camera.
        glm::vec2 worldToWindowPosition(glm::vec2 positionInWorld) const;
        glm::vec2 worldToWindowSize(glm::vec2 sizeInWorld) const;
        // Converts a given position/size from NDC (*1) space to world space, using the camera.
        glm::vec2 ndcToWorldPosition(glm::vec2 positionInNdc) const;
        glm::vec2 ndcToWorldSize(glm::vec2 sizeInNdc) const;
        // Converts a given position/size from world space to NDC (*1) space, using the camera.
        glm::vec2 worldToNdcPosition(glm::vec2 positionInWorld) const;
        glm::vec2 worldToNdcSize(glm::vec2 sizeInWorld) const;

        // Returns the world space coordinates of the left/right/top/bottom edge of camera's view area
        float getLeftEdgeInWorldSpace() const;
        float getRightEdgeInWorldSpace() const;
        float getTopEdgeInWorldSpace() const;
        float getBottomEdgeInWorldSpace() const;
    };




    ///// Notes /////
    // (*1): NDC space is a normalized device coordinate space where X and Y coordinates range from -1 to 1 and the Y axis points upwards.
    //////////////////

} // namespace Renderer2D
}// namespace Pekan