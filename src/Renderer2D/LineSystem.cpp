#include "LineSystem.h"

#include "LineComponent.h"
#include "TransformComponent2D.h"
#include "Utils2D.h"
#include "VerticesAttributeView.h"
#include "PekanLogger.h"

namespace Pekan
{
namespace Renderer2D
{

    void LineSystem::getVertexPositions
    (
        const entt::registry& registry, entt::entity entity,
        void* vertices, int vertexSize, int positionAttributeOffset
    )
    {
        PK_ASSERT(registry.valid(entity), "Trying to get vertex positions of an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<LineComponent>(entity), "Trying to get vertex positions of an entity that doesn't have a LineComponent component.", "Pekan");
        PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to get vertex positions of an entity that doesn't have a TransformComponent2D component.", "Pekan");

        // Get entity's line and transform components
        const LineComponent& line = registry.get<LineComponent>(entity);
        const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);

        // Get world vertex positions by directly applying the transform to line's point A and point B
        const glm::vec2 verticesWorld[2] =
        {
            Utils2D::applyTransform(registry, transform, line.pointA),
            Utils2D::applyTransform(registry, transform, line.pointB)
        };
        // Set world vertex positions into the vertices array
        VerticesAttributeView vertsAttrView = { vertices, 2, vertexSize, positionAttributeOffset };
        vertsAttrView.setVertexAttribute<glm::vec2>(0, verticesWorld[0]);
        vertsAttrView.setVertexAttribute<glm::vec2>(1, verticesWorld[1]);
    }

} // namespace Renderer2D
} // namespace Pekan
