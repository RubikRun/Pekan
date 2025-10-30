#include "ShapeGeometryUtils.h"

#include "VerticesAttributeView.h"
#include "TransformSystem2D.h"

namespace Pekan
{
namespace Renderer2D
{

    void ShapeGeometryUtils::getWorldVertexPositions
    (
        const entt::registry& registry,
        const glm::vec2* localVertexPositions,
        size_t vertexPositionsCount,
        const TransformComponent2D& transform,
        VerticesAttributeView& worldVertexPositions
    )
    {
        // Get world matrix from the transform component
        const glm::mat3& worldMatrix = TransformSystem2D::getWorldMatrix(registry, transform);
        // Apply world matrix to local vertex positions to get world vertex positions
        applyWorldMatrix(worldMatrix, localVertexPositions, vertexPositionsCount, worldVertexPositions);
    }

    void ShapeGeometryUtils::applyWorldMatrix
    (
        const glm::mat3& worldMatrix,
        const glm::vec2* localVertexPositions,
        size_t vertexPositionsCount,
        VerticesAttributeView& worldVertexPositions
    )
    {
        for (int i = 0; i < vertexPositionsCount; i++)
        {
            const glm::vec2 worldPosition = applyWorldMatrix(worldMatrix, localVertexPositions[i]);
            worldVertexPositions.setVertexAttribute<glm::vec2>(i, worldPosition);
        }
    }

	glm::vec2 ShapeGeometryUtils::applyWorldMatrix(const glm::mat3& worldMatrix, const glm::vec2& localPosition)
	{
		return glm::vec2(worldMatrix * glm::vec3(localPosition, 1.0f));
	}

} // namespace Renderer2D
} // namespace Pekan
