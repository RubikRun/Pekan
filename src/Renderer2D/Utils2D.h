#pragma once

#include "TransformComponent2D.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

	struct VerticesAttributeView;

	class Utils2D
	{
	public:

        // Computes world vertex positions for given local vertex positions and a given transform
        static void getWorldVertexPositions
        (
            const entt::registry& registry,
            const glm::vec2* localVertexPositions,             // input array of local vertex positions
            size_t vertexPositionsCount,                       // number of vertex positions in the arrays
            const TransformComponent2D& transform,             // transform component to use for computing world vertex positions
            VerticesAttributeView& worldVertexPositions        // view of the position attribute inside the output array of world vertices
        );

        // Applies a world matrix to an array of local vertex positions to get world vertex positions
        static void applyWorldMatrix
        (
            const glm::mat3& worldMatrix,
            const glm::vec2* localVertexPositions,             // input array of local vertex positions
            size_t vertexPositionsCount,                       // number of vertex positions in the arrays
            VerticesAttributeView& worldVertexPositions        // view of the position attribute inside the output array of world vertices
        );

		// Applies a world matrix to a local position to get world position
		static glm::vec2 applyWorldMatrix(const glm::mat3& worldMatrix, glm::vec2 localPosition);

		// Applies a transform component to a local position to get world position
		static glm::vec2 applyTransform(const entt::registry& registry, const TransformComponent2D& transform, glm::vec2 localPosition);
	};

} // namespace Renderer2D
} // namespace Pekan
