#include "TriangleGeometrySystem.h"

#include "TriangleGeometryComponent.h"
#include "TransformComponent2D.h"
#include "Utils2D.h"
#include "VerticesAttributeView.h"
#include "PekanLogger.h"

namespace Pekan
{
namespace Renderer2D
{

    void TriangleGeometrySystem::getVertexPositionsLocal
    (
        const entt::registry& registry, entt::entity entity,
        void* vertices, int vertexSize, int positionAttributeOffset
    )
    {
        PK_ASSERT(registry.valid(entity), "Cannot get vertex positions of an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<TriangleGeometryComponent>(entity), "Cannot get vertex positions of an entity that doesn't have a TriangleGeometryComponent.", "Pekan");

        // Get entity's geometry component
        const TriangleGeometryComponent& geometry = registry.get<TriangleGeometryComponent>(entity);

        // Local vertex positions are just the 3 points of the triangle geometry
        const glm::vec2 localVertexPositions[3] = { geometry.pointA, geometry.pointB, geometry.pointC };

        // Set local vertex positions into the vertices array using an attribute view
        VerticesAttributeView attributeView{ vertices, 3, vertexSize, positionAttributeOffset };
        for (int i = 0; i < 3; i++)
        {
            attributeView.setVertexAttribute<glm::vec2>(i, localVertexPositions[i]);
        }
    }

	void TriangleGeometrySystem::getVertexPositionsWorld
    (
        const entt::registry& registry, entt::entity entity,
        void* vertices, int vertexSize, int positionAttributeOffset
    )
	{
		PK_ASSERT(registry.valid(entity), "Cannot get vertex positions of an entity that doesn't exist.", "Pekan");
		PK_ASSERT(registry.all_of<TriangleGeometryComponent>(entity), "Cannot get vertex positions of an entity that doesn't have a TriangleGeometryComponent.", "Pekan");
		PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Cannot get vertex positions of an entity that doesn't have a TransformComponent2D.", "Pekan");

		// Get entity's geometry and transform components
		const TriangleGeometryComponent& geometry = registry.get<TriangleGeometryComponent>(entity);
		const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);

		// Local vertex positions are just the 3 points of the triangle geometry
		const glm::vec2 localVertexPositions[3] = { geometry.pointA, geometry.pointB, geometry.pointC };

        // Get world vertex positions using local vertex positions and transform
        VerticesAttributeView attributeView{ vertices, 3, vertexSize, positionAttributeOffset };
        Utils2D::getWorldVertexPositions
        (
            registry,
            localVertexPositions, 3,
            transform,
            attributeView
        );
	}

} // namespace Renderer2D
} // namespace Pekan
