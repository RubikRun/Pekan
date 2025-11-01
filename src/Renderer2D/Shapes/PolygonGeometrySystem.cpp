#include "PolygonGeometrySystem.h"

#include "PolygonGeometryComponent.h"
#include "TransformComponent2D.h"
#include "ShapeGeometryUtils.h"
#include "VerticesAttributeView.h"
#include "PekanLogger.h"

namespace Pekan
{
namespace Renderer2D
{

    void PolygonGeometrySystem::getVertexPositions
    (
        const entt::registry& registry, entt::entity entity,
        void* vertices, int verticesCount, int vertexSize, int positionAttributeOffset
    )
    {
        PK_ASSERT(registry.valid(entity), "Trying to get vertex positions of an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<PolygonGeometryComponent>(entity), "Trying to get vertex positions of an entity that doesn't have a PolygonGeometryComponent component.", "Pekan");
        PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to get vertex positions of an entity that doesn't have a TransformComponent2D component.", "Pekan");

        // Get entity's geometry and transform components
        const PolygonGeometryComponent& geometry = registry.get<PolygonGeometryComponent>(entity);
        const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);

        // Get world vertex positions using geometry's local vertex positions and entity's transform
        ShapeGeometryUtils::getWorldVertexPositions
        (
            registry,
            geometry.vertexPositions.data(),
            geometry.vertexPositions.size(),
            transform,
            VerticesAttributeView{ vertices, verticesCount, vertexSize, positionAttributeOffset }
        );
	}

    int PolygonGeometrySystem::getNumberOfVertices(const entt::registry& registry, entt::entity entity)
    {
        PK_ASSERT(registry.valid(entity), "Trying to get number of vertices for an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<PolygonGeometryComponent>(entity), "Trying to get number of vertices for an entity that doesn't have a PolygonGeometryComponent component.", "Pekan");

        // Get entity's geometry
        const PolygonGeometryComponent& geometry = registry.get<PolygonGeometryComponent>(entity);
		// A polygon's number of vertices is simply the size of its vertex positions array
        return geometry.vertexPositions.size();
    }

} // namespace Renderer2D
} // namespace Pekan
