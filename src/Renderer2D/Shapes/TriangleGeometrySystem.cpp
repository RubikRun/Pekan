#include "TriangleGeometrySystem.h"

#include "TriangleGeometryComponent.h"
#include "TransformComponent2D.h"
#include "ShapeGeometryUtils.h"
#include "VerticesAttributeView.h"
#include "PekanLogger.h"

namespace Pekan
{
namespace Renderer2D
{

	void TriangleGeometrySystem::getVertexPositions
    (
        const entt::registry& registry, entt::entity entity,
        void* vertices, int vertexSize, int positionAttributeOffset
    )
	{
		PK_ASSERT(registry.valid(entity), "Trying to get vertex positions of an entity that doesn't exist.", "Pekan");
		PK_ASSERT(registry.all_of<TriangleGeometryComponent>(entity), "Trying to get vertex positions of an entity that doesn't have a TriangleGeometryComponent component.", "Pekan");
		PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to get vertex positions of an entity that doesn't have a TransformComponent2D component.", "Pekan");

		// Get entity's geometry and transform components
		const TriangleGeometryComponent& geometry = registry.get<TriangleGeometryComponent>(entity);
		const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);

		// Local vertex positions are just the 3 points of the triangle geometry
		const glm::vec2 verticesLocal[3] = { geometry.pointA, geometry.pointB, geometry.pointC };

        // Get world vertex positions using local vertex positions and transform
        ShapeGeometryUtils::getWorldVertexPositions
        (
            registry,
            verticesLocal, 3,
            transform,
            VerticesAttributeView{ vertices, 3, vertexSize, positionAttributeOffset }
        );
	}

} // namespace Renderer2D
} // namespace Pekan
