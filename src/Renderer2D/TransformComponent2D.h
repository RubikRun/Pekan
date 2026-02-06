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

        // Moves the entity by given delta position, adding it to the current position.
        void move(glm::vec2 deltaPosition);
        // Rotates the entity by given delta rotation, adding it to the current rotation.
        void rotate(float deltaRotation);
        // Scales the entity by given delta scale, multiplying it by the current scale factor.
        void scale(glm::vec2 deltaScale);
    };

} // namespace Renderer2D
} // namespace Pekan
