#pragma once

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer
{

    class Camera2D
    {
    public:

        Camera2D(float width, float height);

        void setPosition(const glm::vec2& position);
        void setZoom(float zoom);

        const glm::mat4& getViewProjectionMatrix() const;

    private: /* functions */

        void recalculateView();
        void recalculateProjection();

    private: /* variables */

        glm::vec2 m_position = { 0.0f, 0.0f };
        float m_zoom = 1.0f;

        float m_width;
        float m_height;

        glm::mat4 m_viewMatrix = glm::mat4(1.0f);
        glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
        glm::mat4 m_viewProjectionMatrix = glm::mat4(1.0f);
    };

} // namespace Renderer
} // namespace Pekan