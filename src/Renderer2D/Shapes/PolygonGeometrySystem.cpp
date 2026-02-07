#include "PolygonGeometrySystem.h"

#include "PolygonGeometryComponent.h"
#include "TransformComponent2D.h"
#include "Utils2D.h"
#include "VerticesAttributeView.h"
#include "PekanLogger.h"
#include "Utils/MathUtils.h"

namespace Pekan
{
namespace Renderer2D
{

	// Generates indices for a given array of local vertex positions of a polygon geometry
	void generateIndices
	(
		std::vector<glm::vec2>& localVertexPositions,    // array of local vertex positions of the polygon, might be modified (reversed)
		std::vector<unsigned>& indices                   // output array of indices
	)
	{
		// If polygon is concave (not convex), we need to triangulate it to get indices
		if (!MathUtils::isPolygonConvex(localVertexPositions))
		{
			// Ensure CCW orientation for triangulation
			if (!MathUtils::isPolygonCCW(localVertexPositions))
			{
				std::reverse(localVertexPositions.begin(), localVertexPositions.end());
			}

			// Triangulate polygon, generating indices
			if (!MathUtils::triangulatePolygon(localVertexPositions, indices))
			{
				PK_LOG_ERROR("Failed to triangulate polygon from PolygonGeometryComponent. Possibly self-intersecting.", "Pekan");
				return;
			}
		}
		// Otherwise polygon is convex and we can use triangle fan indices
		else
		{
			const int verticesCount = localVertexPositions.size();
			indices.resize((verticesCount - 2) * 3);
			MathUtils::generateTriangleFanIndices(indices.data(), verticesCount);
		}
	}

	void PolygonGeometrySystem::getVertexPositionsAndIndicesLocal
	(
		const entt::registry& registry, entt::entity entity,
		void* vertices, int verticesCount, int vertexSize, int positionAttributeOffset,
		std::vector<unsigned>& indices
	)
	{
		PK_ASSERT(registry.valid(entity), "Cannot get vertex positions of an entity that doesn't exist.", "Pekan");
		PK_ASSERT(registry.all_of<PolygonGeometryComponent>(entity), "Cannot get vertex positions of an entity that doesn't have a PolygonGeometryComponent.", "Pekan");

		// Get entity's geometry component
		const PolygonGeometryComponent& geometry = registry.get<PolygonGeometryComponent>(entity);

		PK_ASSERT
		(
			geometry.vertexPositions.size() == verticesCount,
			"Number of vertices in given vertices array does not match the number of vertices in entity's PolygonGeometryComponent.", "Pekan"
		);

		// Ensure polygon has at least 3 vertices
		if (geometry.vertexPositions.size() < 3)
		{
			PK_LOG_ERROR("Cannot get vertex positions from a PolygonGeometryComponent with less than 3 vertices.", "Pekan");
			return;
		}

		// Copy geometry's vertex positions into a temporary vector (we might need to reverse them)
		std::vector<glm::vec2> localVertexPositions = geometry.vertexPositions;

		// Generate indices for the polygon
		generateIndices(localVertexPositions, indices);

		// Set local vertex positions into the vertices array using an attribute view
		VerticesAttributeView attributeView{ vertices, int(localVertexPositions.size()), vertexSize, positionAttributeOffset };
		for (int i = 0; i < attributeView.verticesCount; i++)
		{
			attributeView.setVertexAttribute<glm::vec2>(i, localVertexPositions[i]);
		}
	}

	void PolygonGeometrySystem::getVertexPositionsAndIndicesWorld
	(
		const entt::registry& registry, entt::entity entity,
		void* vertices, int verticesCount, int vertexSize, int positionAttributeOffset,
		std::vector<unsigned>& indices
	)
	{
		PK_ASSERT(registry.valid(entity), "Cannot get vertex positions of an entity that doesn't exist.", "Pekan");
		PK_ASSERT(registry.all_of<PolygonGeometryComponent>(entity), "Cannot get vertex positions of an entity that doesn't have a PolygonGeometryComponent.", "Pekan");
		PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Cannot get vertex positions of an entity that doesn't have a TransformComponent2D.", "Pekan");

		// Get entity's geometry and transform components
		const PolygonGeometryComponent& geometry = registry.get<PolygonGeometryComponent>(entity);
		const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);

		PK_ASSERT
		(
			geometry.vertexPositions.size() == verticesCount,
			"Number of vertices in given vertices array does not match the number of vertices in entity's PolygonGeometryComponent.", "Pekan"
		);

		// Ensure polygon has at least 3 vertices
		if (geometry.vertexPositions.size() < 3)
		{
			PK_LOG_ERROR("Cannot get vertex positions from a PolygonGeometryComponent with less than 3 vertices.", "Pekan");
			return;
		}

		// Copy geometry's vertex positions into a local vector (we might need to reverse them)
		std::vector<glm::vec2> localVertexPositions = geometry.vertexPositions;

		// Generate indices for the polygon
		generateIndices(localVertexPositions, indices);

		// Get world vertex positions using local vertex positions and entity's transform
		VerticesAttributeView attributeView{ vertices, verticesCount, vertexSize, positionAttributeOffset };
		Utils2D::getWorldVertexPositions
		(
			registry,
			localVertexPositions.data(), localVertexPositions.size(),
			transform,
			attributeView
		);
	}

} // namespace Renderer2D
} // namespace Pekan
