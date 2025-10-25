#pragma once

#include "Scene.h"
#include <entt/entt.hpp>

namespace Pekan
{
namespace Renderer2D
{

    class SpriteSystem
    {
    public:

        static void render(const Scene& scene);
        static void render(const entt::registry& registry, entt::entity entity);
    };

} // namespace Renderer2D
} // namespace Pekan
