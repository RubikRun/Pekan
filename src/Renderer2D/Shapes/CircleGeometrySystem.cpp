#include "CircleGeometrySystem.h"

#include "CircleGeometryComponent.h"
#include "TransformComponent2D.h"
#include "Utils2D.h"
#include "VerticesAttributeView.h"
#include "PekanLogger.h"
#include "Utils/MathUtils.h"

#include <vector>
#include <glm/gtc/constants.hpp>

constexpr float PI = glm::pi<float>();

namespace Pekan
{
namespace Renderer2D
{

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

    void CircleGeometrySystem::getVertexPositionsAndIndices
    (
        const entt::registry& registry, entt::entity entity,
        void* vertices, int verticesCount, int vertexSize, int positionAttributeOffset,
        std::vector<unsigned>& indices
    )
    {
        PK_ASSERT(registry.valid(entity), "Trying to get vertex positions of an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<CircleGeometryComponent>(entity), "Trying to get vertex positions of an entity that doesn't have a CircleGeometryComponent component.", "Pekan");
        PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to get vertex positions of an entity that doesn't have a TransformComponent2D component.", "Pekan");

        // Get entity's geometry and transform components
        const CircleGeometryComponent& geometry = registry.get<CircleGeometryComponent>(entity);
        const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);

        // Get local vertex positions from geometry
        const std::vector<glm::vec2> localVertexPositions = getLocalVertexPositions(geometry);

        // Get world vertex positions using local vertex positions and transform
        Utils2D::getWorldVertexPositions
        (
            registry,
            localVertexPositions.data(), localVertexPositions.size(),
            transform,
            VerticesAttributeView{ vertices, verticesCount, vertexSize, positionAttributeOffset }
        );

        // Generate triangle fan indices in the indices array
        indices.resize((verticesCount - 2) * 3);
        MathUtils::generateTriangleFanIndices(indices.data(), verticesCount);
    }

    int CircleGeometrySystem::getNumberOfVertices(const entt::registry& registry, entt::entity entity)
    {
        PK_ASSERT(registry.valid(entity), "Trying to get number of vertices for an entity that doesn't exist.", "Pekan");
        PK_ASSERT(registry.all_of<CircleGeometryComponent>(entity), "Trying to get number of vertices for an entity that doesn't have a CircleGeometryComponent component.", "Pekan");

        // Get entity's geometry
        const CircleGeometryComponent& geometry = registry.get<CircleGeometryComponent>(entity);
        // Number of vertices is equal to the number of segments
        // because a polygon of N segments has N vertices
        return geometry.segmentsCount;
    }

} // namespace Renderer2D
} // namespace Pekan
