#include "RectangleGeometrySystem.h"

#include "RectangleGeometryComponent.h"
#include "TransformComponent2D.h"
#include "ShapeGeometryUtils.h"
#include "VerticesAttributeView.h"
#include "PekanLogger.h"

namespace Pekan
{
namespace Renderer2D
{

    // Computes local vertex positions for a given line geometry.
    // @param[in] verticesLocal - output array of local vertices, must have space for 4 elements
    static void getVerticesLocal(const RectangleGeometryComponent& geometry, glm::vec2* verticesLocal)
    {
        verticesLocal[0] = glm::vec2(-geometry.width / 2.0f, -geometry.height / 2.0f);
        verticesLocal[1] = glm::vec2(geometry.width / 2.0f, -geometry.height / 2.0f);
        verticesLocal[2] = glm::vec2(geometry.width / 2.0f, geometry.height / 2.0f);
        verticesLocal[3] = glm::vec2(-geometry.width / 2.0f, geometry.height / 2.0f);
    }

	void RectangleGeometrySystem::getVertexPositions
    (
        const entt::registry& registry, entt::entity entity,
        void* vertices, int vertexSize, int positionAttributeOffset
    )
	{
		PK_ASSERT(registry.valid(entity), "Trying to get vertex positions of an entity that doesn't exist.", "Pekan");
		PK_ASSERT(registry.all_of<RectangleGeometryComponent>(entity), "Trying to get vertex positions of an entity that doesn't have a RectangleGeometryComponent component.", "Pekan");
		PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to get vertex positions of an entity that doesn't have a TransformComponent2D component.", "Pekan");

		// Get entity's geometry and transform components
		const RectangleGeometryComponent& geometry = registry.get<RectangleGeometryComponent>(entity);
		const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);

		// Get local vertex positions from geometry
        glm::vec2 verticesLocal[4];
        getVerticesLocal(geometry, verticesLocal);

        // Get world vertex positions using local vertex positions and transform
        ShapeGeometryUtils::getWorldVertexPositions
        (
            registry,
            verticesLocal, 4,
            transform,
            VerticesAttributeView{ vertices, 4, vertexSize, positionAttributeOffset }
        );
	}

} // namespace Renderer2D
} // namespace Pekan
