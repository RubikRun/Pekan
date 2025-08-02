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

        glm::vec2 getSize() const { return { m_width, m_height }; }
        void setSize(float width, float height);

        // Sets camera's size, in the smaller dimension, to be equal to the given scale.
        // The bigger dimension is automatically adjusted to match the window's aspect ratio.
        void setSize(float scale);

        inline glm::vec2 getPosition() const { return m_position; }
        void setPosition(glm::vec2 position);
        void move(glm::vec2 deltaPosition);

        inline float getZoom() const { return m_zoom; }
        void setZoom(float zoom);
        void zoomIn(float factor);
        void zoomOut(float factor);

        inline const glm::mat4& getViewProjectionMatrix() const { return m_viewProjectionMatrix; };

        // Converts a given position from window space to world space, using the camera.
        glm::vec2 windowToWorld(glm::vec2 windowPosition) const;
        // Converts a given position from NDC (Normalized Device Coordinates) to world space, using the camera
        // (NDC means from -1 to 1 in both the X and Y axes, and the Y axis is pointing upwards)
        glm::vec2 ndcToWorld(glm::vec2 ndcPosition) const;

    private: /* functions */

        void recalculateView();
        void recalculateProjection();

    private: /* variables */

        glm::vec2 m_position = { 0.0f, 0.0f };
        float m_zoom = 1.0f;

        float m_width = -1.0f;
        float m_height = -1.0f;

        glm::mat4 m_viewMatrix = glm::mat4(1.0f);
        glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
        glm::mat4 m_viewProjectionMatrix = glm::mat4(1.0f);
    };

    typedef std::shared_ptr<Renderer2D::Camera2D> Camera2D_Ptr;
    typedef std::shared_ptr<const Renderer2D::Camera2D> Camera2D_ConstPtr;
    typedef std::weak_ptr<Renderer2D::Camera2D> Camera2D_WeakPtr;
    typedef std::weak_ptr<const Renderer2D::Camera2D> Camera2D_ConstWeakPtr;

} // namespace Renderer2D
} // namespace Pekan
