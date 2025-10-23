#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace Pekan
{
namespace Renderer2D
{

    struct TransformComponent2D
    {
        glm::vec2 position = { 0.0f, 0.0f };
        float rotation = 0.0f;                  // in radians
        glm::vec2 scale = { 1.0f, 1.0f };

        entt::entity parent = entt::null;
    };

} // namespace Renderer2D
} // namespace Pekan
