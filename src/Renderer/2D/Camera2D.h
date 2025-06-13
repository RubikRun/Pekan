#pragma once

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer
{

    class Camera2D
    {
    public:

        void setSize(float width, float height);

        inline glm::vec2 getPosition() const { return m_position; }
        void setPosition(glm::vec2 position);
        void move(glm::vec2 deltaPosition);

        inline float getZoom() const { return m_zoom; }
        void setZoom(float zoom);
        void zoomIn(float factor);
        void zoomOut(float factor);

        const glm::mat4& getViewProjectionMatrix() const;

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

} // namespace Renderer
} // namespace Pekan