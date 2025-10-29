#include "CircleGeometrySystem.h"

#include "CircleGeometryComponent.h"
#include "TransformComponent2D.h"
#include "TransformSystem2D.h"
#include "PekanLogger.h"

#include <vector>
#include <glm/gtc/constants.hpp>

constexpr float PI = glm::pi<float>();

namespace Pekan
{
namespace Renderer2D
{

	// Structure defining a view of a vertex attribute inside a vertices array
    struct VerticesAttributeView
    {
		void* vertices;	              // pointer to an array of vertices
		int verticesCount;            // number of vertices in the array
        int vertexSize;	              // size of a single vertex, in bytes
		int offsetFromVertexStart;    // offset from the start of each vertex to a the attribute, in bytes

		// Sets the attribute of a given vertex to a given value
        template <typename T>
        void setVertexAttribute
        (
            int vertexIndex,    // index of the vertex whose attribute to set
            T attrValue         // attribute value to set
        )
        {
            char* vertexPtr = static_cast<char*>(vertices) + vertexIndex * vertexSize;
            *reinterpret_cast<T*>(vertexPtr + offsetFromVertexStart) = attrValue;
        }
    };

	// Applies a world matrix to a local position to get the world position
    static glm::vec2 applyWorldMatrix(const glm::mat3& worldMatrix, const glm::vec2& localPosition)
    {
        return glm::vec2(worldMatrix * glm::vec3(localPosition, 1.0f));
	}

    // Applies a world matrix to an array of local vertex positions to get world vertex positions
    static void applyWorldMatrix
    (
        const glm::mat3& worldMatrix,
        const glm::vec2* localVertexPositions,             // input array of local vertex positions
        size_t vertexPositionsCount,                       // number of vertex positions in the arrays
		VerticesAttributeView& worldVertexPositions        // view of the position attribute inside the output array of world vertices
    )
    {
        for (int i = 0; i < vertexPositionsCount; i++)
        {
			const glm::vec2 worldPosition = applyWorldMatrix(worldMatrix, localVertexPositions[i]);
            worldVertexPositions.setVertexAttribute<glm::vec2>(i, worldPosition);
        }
	}

    // Computes local vertex positions for a given circle geometry
    static std::vector<glm::vec2> getLocalVertexPositions(const CircleGeometryComponent& geometry)
    {
		std::vector<glm::vec2> localVertexPositions(geometry.segmentsCount);
        for (int i = 0; i < geometry.segmentsCount; i++)
        {
            const float angle = float(i) * 2.0f * PI / geometry.segmentsCount;
            const float x = geometry.radius * cos(angle);
            const float y = geometry.radius * sin(angle);
            localVertexPositions[i] = { x, y };
        }
        return localVertexPositions;
    }

	// Computes world vertex positions for a given circle geometry and transform
    static void getWorldVertexPositions
    (
        const entt::registry& registry,
        const CircleGeometryComponent& geometry,
        const TransformComponent2D& transform,
		VerticesAttributeView& worldVertexPositions    // view of the position attribute inside the output array of world vertices
    )
    {
        // Get local vertex positions of the circle geometry
        std::vector<glm::vec2> localVertexPositions = getLocalVertexPositions(geometry);
		// Get world matrix from the transform component
        const glm::mat3& worldMatrix = TransformSystem2D::getWorldMatrix(registry, transform);
		// Apply world matrix to local vertex positions to get world vertex positions
        applyWorldMatrix(worldMatrix, localVertexPositions.data(), localVertexPositions.size(), worldVertexPositions);
    }

	void Renderer2D::CircleGeometrySystem::getVertexPositions
    (
        const entt::registry& registry,
        entt::entity entity,
        void* vertices,
        int verticesCount,
        int vertexSize,
        int positionAttributeOffset
    )
	{
		PK_ASSERT(registry.valid(entity), "Trying to get vertex positions of an entity that doesn't exist.", "Pekan");
		PK_ASSERT(registry.all_of<CircleGeometryComponent>(entity), "Trying to get vertex positions of an entity that doesn't have a CircleGeometryComponent component.", "Pekan");
		PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to get vertex positions of an entity that doesn't have a TransformComponent2D component.", "Pekan");

		// Get entity's geometry and transform components
		const CircleGeometryComponent& geometry = registry.get<CircleGeometryComponent>(entity);
		const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);
		// Get world vertex positions using geometry and transform
        getWorldVertexPositions
        (
            registry,
            geometry,
            transform,
            VerticesAttributeView { vertices, verticesCount, vertexSize, positionAttributeOffset }
        );
	}

    int CircleGeometrySystem::getNumberOfVertices(const entt::registry& registry, entt::entity entity)
    {
        PK_ASSERT(registry.valid(entity), "Trying to get number of vertices for an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<CircleGeometryComponent>(entity), "Trying to get number of vertices for an entity that doesn't have a CircleGeometryComponent component.", "Pekan");

        // Get entity's geometry
        const CircleGeometryComponent& geometry = registry.get<CircleGeometryComponent>(entity);
		// Number of vertices is equal to the number of segments
        // because a polygon of N segments has N vertices.
        return geometry.segmentsCount;
    }

} // namespace Renderer2D
} // namespace Pekan
