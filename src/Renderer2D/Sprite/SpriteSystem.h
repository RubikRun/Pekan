#pragma once

#include "Texture2D.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

    class SpriteSystem
    {
    public:

        static void render(const entt::registry& registry);
        static void render(const entt::registry& registry, entt::entity entity);

		static float                           getWidth(const entt::registry& registry, entt::entity entity);
		static float                           getHeight(const entt::registry& registry, entt::entity entity);
		static glm::vec2                       getSize(const entt::registry& registry, entt::entity entity);
		static Graphics::Texture2D_ConstPtr    getTexture(const entt::registry& registry, entt::entity entity);
		static glm::vec2                       getTextureCoordinatesMin(const entt::registry& registry, entt::entity entity);
		static glm::vec2                       getTextureCoordinatesMax(const entt::registry& registry, entt::entity entity);
    };

} // namespace Renderer2D
} // namespace Pekan
