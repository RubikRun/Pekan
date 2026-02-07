#include "RectangleGeometrySystem.h"

#include "RectangleGeometryComponent.h"
#include "TransformComponent2D.h"
#include "Utils2D.h"
#include "VerticesAttributeView.h"
#include "PekanLogger.h"

namespace Pekan
{
namespace Renderer2D
{

	// Computes local vertex positions for a given line geometry.
	// @param[in] localVertexPositions - output array of local vertex positions, must have space for 4 elements
	static void getLocalVertexPositions(const RectangleGeometryComponent& geometry, glm::vec2* localVertexPositions)
	{
		localVertexPositions[0] = glm::vec2(-geometry.width / 2.0f, -geometry.height / 2.0f);
		localVertexPositions[1] = glm::vec2(geometry.width / 2.0f, -geometry.height / 2.0f);
		localVertexPositions[2] = glm::vec2(geometry.width / 2.0f, geometry.height / 2.0f);
		localVertexPositions[3] = glm::vec2(-geometry.width / 2.0f, geometry.height / 2.0f);
	}

	void RectangleGeometrySystem::getVertexPositionsLocal
	(
		const entt::registry& registry, entt::entity entity,
		void* vertices, int vertexSize, int positionAttributeOffset
	)
	{
		PK_ASSERT(registry.valid(entity), "Cannot get vertex positions of an entity that doesn't exist.", "Pekan");
		PK_ASSERT(registry.all_of<RectangleGeometryComponent>(entity), "Cannot get vertex positions of an entity that doesn't have a RectangleGeometryComponent.", "Pekan");

		// Get entity's geometry component
		const RectangleGeometryComponent& geometry = registry.get<RectangleGeometryComponent>(entity);

		// Get local vertex positions from geometry
		glm::vec2 localVertexPositions[4];
		getLocalVertexPositions(geometry, localVertexPositions);

		// Set local vertex positions into the vertices array using an attribute view
		VerticesAttributeView attributeView{ vertices, 4, vertexSize, positionAttributeOffset };
		for (int i = 0; i < 4; i++)
		{
			attributeView.setVertexAttribute<glm::vec2>(i, localVertexPositions[i]);
		}
	}

	void RectangleGeometrySystem::getVertexPositionsWorld
	(
		const entt::registry& registry, entt::entity entity,
		void* vertices, int vertexSize, int positionAttributeOffset
	)
	{
		PK_ASSERT(registry.valid(entity), "Cannot get vertex positions of an entity that doesn't exist.", "Pekan");
		PK_ASSERT(registry.all_of<RectangleGeometryComponent>(entity), "Cannot get vertex positions of an entity that doesn't have a RectangleGeometryComponent.", "Pekan");
		PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Cannot get vertex positions of an entity that doesn't have a TransformComponent2D.", "Pekan");

		// Get entity's geometry and transform components
		const RectangleGeometryComponent& geometry = registry.get<RectangleGeometryComponent>(entity);
		const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);

		// Get local vertex positions from geometry
		glm::vec2 localVertexPositions[4];
		getLocalVertexPositions(geometry, localVertexPositions);

		// Get world vertex positions using local vertex positions and transform
		VerticesAttributeView attributeView{ vertices, 4, vertexSize, positionAttributeOffset };
		Utils2D::getWorldVertexPositions
		(
			registry,
			localVertexPositions, 4,
			transform,
			attributeView
		);
	}

} // namespace Renderer2D
} // namespace Pekan
