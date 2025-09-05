#pragma once

#include <glm/glm.hpp>
#include <memory>

namespace Pekan
{
namespace Renderer2D
{

    class Camera2D
    {
    public:

        ~Camera2D();

        // Creates a camera with a given width and height
        // @param[in] width - Camera's width in world space. This is the width of the area visible by the camera.
        // @param[in] height - Camera's height in world space. This is the height of the area visible by the camera.
        void create(float width, float height);
        // Creates a camera with a given scale, which will be equal to the smaller dimension of camera's size.
        // The bigger dimension is automatically adjusted to match the window's aspect ratio.
        void create(float scale);
        void destroy();

        // Returns camera's size in world space. This is the size of the area visible by the camera
        glm::vec2 getSize() const { return { m_width, m_height }; }
        // Sets camera's size in world space. This is the size of the area visible by the camera
        void setSize(float width, float height);

        // Sets camera's scale, which will be equal to the smaller dimension.
        // The bigger dimension is automatically adjusted to match the window's aspect ratio.
        void setSize(float scale);

        // Returns camera's position in world space. This is the center of the area visible by the camera.
        glm::vec2 getPosition() const { return m_position; }
        // Sets camera's position in world space. This is the center of the area visible by the camera.
        void setPosition(glm::vec2 position);
        // Moves camera by some amount in world space.
        void move(glm::vec2 deltaPosition);

        // Returns camera's zoom level. This is a factor by which camera's size (width and height) is divided before use.
        float getZoom() const { return m_zoom; }
        // Sets camera's zoom level. This is a factor by which camera's size (width and height) is divided before use.
        void setZoom(float zoom);

        // Zooms in with the camera, multiplying the current zoom level by some factor.
        void zoomIn(float factor);
        // Zooms out with the camera, dividing the current zoom level by some factor.
        void zoomOut(float factor);

        // Returns camera's view projection matrix.
        // The view projection matrix encapsulates camera's size, position and zoom level.
        // It can be used to multiply a world-space point to get the corresponding NDC (-1 to 1) point.
        const glm::mat4& getViewProjectionMatrix() const;

        // Converts a given position from window space to world space, using the camera.
        glm::vec2 windowToWorld(glm::vec2 windowPosition) const;
        // Converts a given position from world space to window space, using the camera.
        glm::vec2 worldToWindow(glm::vec2 worldPosition) const;
        // Converts a given position from NDC (Normalized Device Coordinates) to world space, using the camera
        // (NDC means from -1 to 1 in both the X and Y axes, and the Y axis is pointing upwards)
        glm::vec2 ndcToWorld(glm::vec2 ndcPosition) const;
        // Converts a given world position to NDC (Normalized Device Coordinates)
        glm::vec2 worldToNdc(glm::vec2 worldPosition) const;

        // Checks if camera is valid, meaning that it has been created and not yet destroyed.
        bool isValid() const { return m_isValid; }

    private: /* functions */

        // Recalculates camera's view matrix.
        void updateViewMatrix() const;
        // Recalculates camera's projection matrix.
        void updateProjectionMatrix() const;

    private: /* variables */

        // Camera's width in world space.
        // This is the width of the area visible by the camera.
        float m_width = -1.0f;
        // Camera's height in world space.
        // This is the height of the area visible by the camera.
        float m_height = -1.0f;

        // Camera's position in world space.
        // This is the center of the area visible by the camera.
        glm::vec2 m_position = { 0.0f, 0.0f };

        // Camera's zoom level.
        // This is a factor by which camera's size (width and height) is divided before use.
        // For example:
        // - a zoom level of 2 means that camera's size will be divided by 2 so the visible area will be 2 times SMALLER in each dimension,
        //   which has the effect of "zooming in".
        // - a zoom level of 0.5 means that camera's size will be divided by 0.5 so the visible area will be 2 times BIGGER in each dimension,
        //   which has the effect of "zooming out".
        float m_zoom = 1.0f;

        // A view matrix making up the "view" part of the view projection matrix
        mutable glm::mat4 m_viewMatrix = glm::mat4(1.0f);
        // A projection matrix making up the "projection" part of the view projection matrix
        mutable glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
        // A view projection matrix encapsulating camera's size, position and zoom level.
        // This matrix can be used to multiply a world-space point to get the corresponding NDC (-1 to 1) point.
        mutable glm::mat4 m_viewProjectionMatrix = glm::mat4(1.0f);

        // Flag indicating if we need to update the view matrix before use.
        mutable bool m_needUpdateViewMatrix = false;
        // Flag indicating if we need to update the projection matrix before use.
        mutable bool m_needUpdateProjectionMatrix = false;

        // Flag indicating if camera is valid, meaning that it has been created and not yet destroyed
        bool m_isValid = false;
    };

    typedef std::shared_ptr<Renderer2D::Camera2D> Camera2D_Ptr;
    typedef std::shared_ptr<const Renderer2D::Camera2D> Camera2D_ConstPtr;
    typedef std::weak_ptr<Renderer2D::Camera2D> Camera2D_WeakPtr;
    typedef std::weak_ptr<const Renderer2D::Camera2D> Camera2D_ConstWeakPtr;

} // namespace Renderer2D
} // namespace Pekan
