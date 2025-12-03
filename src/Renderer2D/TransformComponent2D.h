#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace Pekan
{
namespace Renderer2D
{

    struct TransformComponent2D
    {
    /* data */

        glm::vec2 position = { 0.0f, 0.0f };
        float rotation = 0.0f;                  // in radians
        glm::vec2 scaleFactor = { 1.0f, 1.0f };

        entt::entity parent = entt::null;

    /* functions */

        void move(glm::vec2 deltaPosition);
        void rotate(float deltaRotation);
        void scale(glm::vec2 deltaScale);
    };

} // namespace Renderer2D
} // namespace Pekan
