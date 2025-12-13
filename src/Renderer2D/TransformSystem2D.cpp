#include "TransformSystem2D.h"

#include "TransformComponent2D.h"
#include "PekanLogger.h"

namespace Pekan
{
namespace Renderer2D
{

    static glm::mat3 getLocalMatrix(const TransformComponent2D& transform)
    {
        const float cosRot = cos(transform.rotation);
        const float sinRot = sin(transform.rotation);

        const glm::mat3 scaleMatrix = glm::mat3
        (
            glm::vec3(transform.scaleFactor.x, 0.0f, 0.0f),
            glm::vec3(0.0f, transform.scaleFactor.y, 0.0f),
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
        PK_ASSERT(registry.valid(entity), "Cannot get world matrix of an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Cannot get world matrix of an entity that doesn't have a TransformComponent2D.", "Pekan");

        const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);
		return getWorldMatrix(registry, transform);
    }

    glm::mat3 TransformSystem2D::getWorldMatrix(const entt::registry& registry, const TransformComponent2D& transform)
    {
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
