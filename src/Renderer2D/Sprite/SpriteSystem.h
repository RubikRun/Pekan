#pragma once

#include "Texture2D.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{
	struct CameraComponent2D;

    class SpriteSystem
    {
    public:

        static void render(const entt::registry& registry, const CameraComponent2D* camera);
    };

} // namespace Renderer2D
} // namespace Pekan
