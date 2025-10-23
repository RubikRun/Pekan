#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace Pekan
{
namespace Renderer2D
{

    class TransformSystem2D
    {
    public:

        static glm::vec2 getPosition(const entt::registry& registry, entt::entity entity);
        static float getRotation(const entt::registry& registry, entt::entity entity);
        static glm::vec2 getScale(const entt::registry& registry, entt::entity entity);

        static void setPosition(entt::registry& registry, entt::entity entity, glm::vec2 position);
        static void setRotation(entt::registry& registry, entt::entity entity, float rotation);
        static void setScale(entt::registry& registry, entt::entity entity, glm::vec2 scale);

        static void move(entt::registry& registry, entt::entity entity, glm::vec2 deltaPosition);
        static void rotate(entt::registry& registry, entt::entity entity, float deltaRotation);
        static void scale(entt::registry& registry, entt::entity entity, glm::vec2 deltaScale);

        static glm::mat3 getWorldMatrix(const entt::registry& registry, entt::entity entity);
    };

} // namespace Renderer2D
} // namespace Pekan
