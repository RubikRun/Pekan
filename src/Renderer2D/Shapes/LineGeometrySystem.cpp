#include "LineGeometrySystem.h"

#include "LineGeometryComponent.h"
#include "TransformComponent2D.h"
#include "Utils2D.h"
#include "VerticesAttributeView.h"
#include "PekanLogger.h"

namespace Pekan
{
namespace Renderer2D
{

    // Computes local vertex positions for a given line geometry.
    // @param[in] verticesLocal - output array of local vertices, must have space for 4 elements
    static void getVerticesLocal(const LineGeometryComponent& geometry, glm::vec2* verticesLocal)
    {
		const glm::vec2& a = geometry.pointA;
		const glm::vec2& b = geometry.pointB;
		const float& t = geometry.thickness;
        // Calculate the normal offset vector.
        // We'll use that vector to "thicken" the line to the required thickness.
        // 
        // The ideal line AB has 0 thickness.
        // To make it have thickness T we need to turn the 2 points A and B into 4 points.
        // Point A will turn into 2 points, and point B will turn into 2 points.
        // The way to turn an original point into 2 new points is to add a vector of length T / 2 in the 2 directions perpendicular to AB.
        // The normal offset vector is just that - a vector in one of the perpendicular directions with length T / 2.
        // The vector in the other perpendicular direction will be just the negative of that vector
        const glm::vec2 dir = glm::normalize(a - b);
        const glm::vec2 normalOffset
        (
            -(t / 2.0f) * dir.y,
            (t / 2.0f) * dir.x
        );
		// Add and substract the normal offset vector to points A and B such that the vertices are in counter-clockwise order
        verticesLocal[0] = a + normalOffset;
        verticesLocal[1] = a - normalOffset;
        verticesLocal[2] = b - normalOffset;
        verticesLocal[3] = b + normalOffset;
    }

	void LineGeometrySystem::getVertexPositions
    (
        const entt::registry& registry, entt::entity entity,
        void* vertices, int vertexSize, int positionAttributeOffset
    )
	{
		PK_ASSERT(registry.valid(entity), "Trying to get vertex positions of an entity that doesn't exist.", "Pekan");
		PK_ASSERT(registry.all_of<LineGeometryComponent>(entity), "Trying to get vertex positions of an entity that doesn't have a LineGeometryComponent component.", "Pekan");
		PK_ASSERT(registry.all_of<TransformComponent2D>(entity), "Trying to get vertex positions of an entity that doesn't have a TransformComponent2D component.", "Pekan");

		// Get entity's geometry and transform components
		const LineGeometryComponent& geometry = registry.get<LineGeometryComponent>(entity);
		const TransformComponent2D& transform = registry.get<TransformComponent2D>(entity);

		// Get local vertex positions from geometry
        glm::vec2 verticesLocal[4];
        getVerticesLocal(geometry, verticesLocal);

        // Get world vertex positions using local vertex positions and transform
        Utils2D::getWorldVertexPositions
        (
            registry,
            verticesLocal, 4,
            transform,
            VerticesAttributeView{ vertices, 4, vertexSize, positionAttributeOffset }
        );
	}

} // namespace Renderer2D
} // namespace Pekan
