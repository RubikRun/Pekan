#include "SolidColorMaterialSystem.h"

#include "SolidColorMaterialComponent.h"
#include "PekanLogger.h"

namespace Pekan
{
namespace Renderer2D
{

	// Sets the color of a vertex in a given vertices array
    static void setVertexColor
    (
		int index,                          // index of the vertex whose color to set
        const glm::vec4& vertexColor,       // vertex color to set
        void* vertices,                     // output array of vertices
        int vertexSize,                     // size of a single vertex, in bytes
		int offsetFromVertexStart           // offset from the start of each vertex to the color data, in bytes
    )
    {
        char* vertexPtr = static_cast<char*>(vertices) + index * vertexSize + offsetFromVertexStart;
        *reinterpret_cast<glm::vec4*>(vertexPtr) = vertexColor;
	}

	void Renderer2D::SolidColorMaterialSystem::fillVertexColors
    (
        const entt::registry& registry,
        entt::entity entity,
        void* vertices,
        int verticesCount,
        int vertexSize,
        int offsetFromVertexStart
    )
	{
		PK_ASSERT(registry.valid(entity), "Trying to get vertex colors of an entity that doesn't exist.", "Pekan");
		PK_ASSERT(registry.all_of<SolidColorMaterialComponent>(entity), "Trying to get vertex colors of an entity that doesn't have a SolidColorMaterialComponent component.", "Pekan");

		// Get entity's material component
		const SolidColorMaterialComponent& material = registry.get<SolidColorMaterialComponent>(entity);
        // Set vertex colors
        for (int i = 0; i < verticesCount; i++)
        {
			setVertexColor(i, material.color, vertices, vertexSize, offsetFromVertexStart);
        }
	}

} // namespace Renderer2D
} // namespace Pekan
