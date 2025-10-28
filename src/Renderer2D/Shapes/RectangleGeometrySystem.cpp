#include "RectangleGeometrySystem.h"

#include "RectangleGeometryComponent.h"
#include "TransformComponent2D.h"
#include "TransformSystem2D.h"
#include "PekanLogger.h"

namespace Pekan
{
namespace Renderer2D
{

	// Sets the position of a vertex in a given vertices array
    static void setVertexPosition
    (
		int index,                          // index of the vertex whose position to set
        const glm::vec2& vertexPosition,    // vertex position to set
        void* vertices,                     // output array of vertices
        int vertexSize,                     // size of a single vertex, in bytes
		int offsetFromVertexStart           // offset from the start of each vertex to the position data, in bytes
    )
    {
        char* vertexPtr = static_cast<char*>(vertices) + index * vertexSize + offsetFromVertexStart;
        *reinterpret_cast<glm::vec2*>(vertexPtr) = vertexPosition;
	}

	// Applies a world matrix to a local position to get the world position
    static glm::vec2 applyWorldMatrix(const glm::mat3& worldMatrix, const glm::vec2& localPosition)
    {
        return glm::vec2(worldMatrix * glm::vec3(localPosition, 1.0f));
	}

    // Computes local vertex positions for a given rectangle geometry.
	// Given array of local vertices must have space for 4 vec2 elements.
    static void getVerticesLocal(const RectangleGeometryComponent& geometry, glm::vec2* verticesLocal)
    {
        verticesLocal[0] = glm::vec2(-geometry.width / 2.0f, -geometry.height / 2.0f);
        verticesLocal[1] = glm::vec2(geometry.width / 2.0f, -geometry.height / 2.0f);
        verticesLocal[2] = glm::vec2(geometry.width / 2.0f, geometry.height / 2.0f);
        verticesLocal[3] = glm::vec2(-geometry.width / 2.0f, geometry.height / 2.0f);
    }

	// Computes world vertices for a given rectangle geometry and transform.
    static void getVerticesWorld
    (
        const entt::registry& registry,
        const RectangleGeometryComponent& geometry,
        const TransformComponent2D& transform,
        void* vertices,              // output array of vertices
        int vertexSize,              // size of a single vertex, in bytes
        int offsetFromVertexStart    // offset from the start of each vertex to the position data, in bytes
    )
    {
        glm::vec2 verticesLocal[4];
        getVerticesLocal(geometry, verticesLocal);

        const glm::mat3& worldMatrix = TransformSystem2D::getWorldMatrix(registry, transform);
        // Calculate world vertex positions by applying the world matrix to the local vertex positions
        setVertexPosition(0, applyWorldMatrix(worldMatrix, verticesLocal[0]), vertices, vertexSize, offsetFromVertexStart);
        setVertexPosition(1, applyWorldMatrix(worldMatrix, verticesLocal[1]), vertices, vertexSize, offsetFromVertexStart);
        setVertexPosition(2, applyWorldMatrix(worldMatrix, verticesLocal[2]), vertices, vertexSize, offsetFromVertexStart);
        setVertexPosition(3, applyWorldMatrix(worldMatrix, verticesLocal[3]), vertices, vertexSize, offsetFromVertexStart);
    }

	void Renderer2D::RectangleGeometrySystem::fillVertexPositions
    (
        const entt::registry& registry,
        entt::entity entity,
        void* vertices,
        int vertexSize,
        int offsetFromVertexStart
    )
	{
		PK_ASSERT(registry.valid(entity), "Trying to get vertex positions of an entity that doesn't exist.", "Pekan");
		PK_ASSERT(registry.all_of<RectangleGeometryComponent>(entity), "Trying to get vertex positions of an entity that doesn't have a RectangleGeometryComponent component.", "Pekan");
		PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to get vertex positions of an entity that doesn't have a TransformComponent2D component.", "Pekan");

		// Get entity's geometry and transform components
		const RectangleGeometryComponent& geometry = registry.get<RectangleGeometryComponent>(entity);
		const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);

		// Get world vertices
		getVerticesWorld(registry, geometry, transform, vertices, vertexSize, offsetFromVertexStart);
	}

} // namespace Renderer2D
} // namespace Pekan
