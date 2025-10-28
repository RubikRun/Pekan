#pragma once

#include <entt/entt.hpp>

namespace Pekan
{
namespace Renderer2D
{

    class SpriteSystem
    {
    public:

        static void render(const entt::registry& registry);
        static void render(const entt::registry& registry, entt::entity entity);
    };

} // namespace Renderer2D
} // namespace Pekan
