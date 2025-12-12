#pragma once

#include <entt/entt.hpp>

namespace Pekan
{
namespace Renderer2D
{

    class RenderSystem2D
    {
    public:

        // Renders all renderable entities in the given registry
        static void render(const entt::registry& registry);
    };

} // namespace Renderer2D
} // namespace Pekan
