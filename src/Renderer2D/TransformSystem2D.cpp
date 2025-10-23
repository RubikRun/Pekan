#include "TransformSystem2D.h"

#include "TransformComponent2D.h"
#include "PekanLogger.h"

namespace Pekan
{
namespace Renderer2D
{

    void TransformSystem2D::setPosition(entt::registry& registry, entt::entity entity, glm::vec2 position)
    {
        PK_ASSERT(registry.valid(entity), "Trying to set position of an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to set position of an entity that doesn't have a TransformComponent2D component.", "Pekan");

        TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);
        transform.position = position;
    }

    void TransformSystem2D::setRotation(entt::registry& registry, entt::entity entity, float rotation)
    {
        PK_ASSERT(registry.valid(entity), "Trying to set rotation of an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to set rotation of an entity that doesn't have a TransformComponent2D component.", "Pekan");

        TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);
        transform.rotation = rotation;
    }

    void TransformSystem2D::setScale(entt::registry& registry, entt::entity entity, glm::vec2 scale)
    {
        PK_ASSERT(registry.valid(entity), "Trying to set scale of an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to set scale of an entity that doesn't have a TransformComponent2D component.", "Pekan");

        TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);
        transform.scale = scale;
    }

    void TransformSystem2D::move(entt::registry& registry, entt::entity entity, glm::vec2 deltaPosition)
    {
        PK_ASSERT(registry.valid(entity), "Trying to move an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to move an entity that doesn't have a TransformComponent2D component.", "Pekan");

        TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);
        transform.position += deltaPosition;
    }

    void TransformSystem2D::rotate(entt::registry& registry, entt::entity entity, float deltaRotation)
    {
        PK_ASSERT(registry.valid(entity), "Trying to rotate an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to rotate an entity that doesn't have a TransformComponent2D component.", "Pekan");

        TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);
        transform.rotation += deltaRotation;
    }

    void TransformSystem2D::scale(entt::registry& registry, entt::entity entity, glm::vec2 deltaScale)
    {
        PK_ASSERT(registry.valid(entity), "Trying to scale an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to scale an entity that doesn't have a TransformComponent2D component.", "Pekan");

        TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);
        transform.scale *= deltaScale;
    }

    glm::vec2 TransformSystem2D::getPosition(const entt::registry& registry, entt::entity entity)
    {
        PK_ASSERT(registry.valid(entity), "Trying to get position of an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to get position of an entity that doesn't have a TransformComponent2D component.", "Pekan");

        const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);
        return transform.position;
    }

    float TransformSystem2D::getRotation(const entt::registry& registry, entt::entity entity)
    {
        PK_ASSERT(registry.valid(entity), "Trying to get rotation of an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to get rotation of an entity that doesn't have a TransformComponent2D component.", "Pekan");

        const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);
        return transform.rotation;
    }

    glm::vec2 TransformSystem2D::getScale(const entt::registry& registry, entt::entity entity)
    {
        PK_ASSERT(registry.valid(entity), "Trying to get scale of an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to get scale of an entity that doesn't have a TransformComponent2D component.", "Pekan");

        const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);
        return transform.scale;
    }

    static glm::mat3 getLocalMatrix(const TransformComponent2D& transform)
    {
        const float cosRot = cos(transform.rotation);
        const float sinRot = sin(transform.rotation);

        const glm::mat3 scaleMatrix = glm::mat3
        (
            glm::vec3(transform.scale.x, 0.0f, 0.0f),
            glm::vec3(0.0f, transform.scale.y, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );
        const glm::mat3 rotationMatrix = glm::mat3
        (
            glm::vec3(cosRot, -sinRot, 0.0f),
            glm::vec3(sinRot, cosRot, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );
        const glm::mat3 translationMatrix = glm::mat3
        (
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(transform.position.x, transform.position.y, 1.0f)
        );

        const glm::mat3 localMatrix = translationMatrix * rotationMatrix * scaleMatrix;
        return localMatrix;
    }

    glm::mat3 TransformSystem2D::getWorldMatrix(const entt::registry& registry, entt::entity entity)
    {
        PK_ASSERT(registry.valid(entity), "Trying to get world matrix of an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to get world matrix of an entity that doesn't have a TransformComponent2D component.", "Pekan");

        const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);

        // Get local matrix
        const glm::mat3 localMatrix = getLocalMatrix(transform);
		// If there is a parent, get parent's world matrix and multiply
        if (transform.parent != entt::null && registry.all_of<TransformComponent2D>(transform.parent))
        {
			const glm::mat3 parentWorldMatrix = getWorldMatrix(registry, transform.parent);
            const glm::mat3 worldMatrix = parentWorldMatrix * localMatrix;
            return worldMatrix;
        }

        return localMatrix;
    }

} // namespace Renderer2D
} // namespace Pekan
