#pragma once

#include <entt/entt.hpp>

namespace Pekan
{
namespace Renderer2D
{

    class SolidColorMaterialSystem
    {
    public:

        // Retrieves the color from given entity's SolidColorMaterialComponent
        // and sets it as the color attribute of each vertex in the given array of vertices.
        static void getVertexColors
        (
            const entt::registry& registry,
            entt::entity entity,
            void* vertices,             // output array of vertices
            int verticesCount,          // number of vertices in the array
            int vertexSize,             // size of a single vertex, in bytes
            int colorAttributeOffset    // offset from the start of each vertex to the color attribute, in bytes
        );
    };

} // namespace Renderer2D
} // namespace Pekan
