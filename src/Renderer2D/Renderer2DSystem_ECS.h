#pragma once

#include <entt/entt.hpp>

namespace Pekan
{
namespace Renderer2D
{

    class Renderer2DSystem_ECS
    {
    public:

        // Renders all renderable entities in the given registry
        static void render(const entt::registry& registry);
    };

} // namespace Renderer2D
} // namespace Pekan
