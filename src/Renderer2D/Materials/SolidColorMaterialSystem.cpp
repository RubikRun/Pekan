#include "SolidColorMaterialSystem.h"

#include "SolidColorMaterialComponent.h"
#include "VerticesAttributeView.h"
#include "PekanLogger.h"

namespace Pekan
{
namespace Renderer2D
{

	void Renderer2D::SolidColorMaterialSystem::getVertexColors
    (
        const entt::registry& registry,
        entt::entity entity,
        void* vertices, int verticesCount, int vertexSize, int colorAttributeOffset
    )
	{
		PK_ASSERT(registry.valid(entity), "Trying to get vertex colors of an entity that doesn't exist.", "Pekan");
		PK_ASSERT(registry.all_of<SolidColorMaterialComponent>(entity), "Trying to get vertex colors of an entity that doesn't have a SolidColorMaterialComponent component.", "Pekan");

		// Get entity's material component
		const SolidColorMaterialComponent& material = registry.get<SolidColorMaterialComponent>(entity);
        // Set vertex colors
		VerticesAttributeView colorAttrView { vertices, verticesCount, vertexSize, colorAttributeOffset };
        for (int i = 0; i < verticesCount; i++)
        {
			colorAttrView.setVertexAttribute<glm::vec4>(i, material.color);
        }
	}

} // namespace Renderer2D
} // namespace Pekan
