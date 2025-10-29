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
        // and sets it on the color attribute of each vertex in the given vertices array.
        static void getVertexColors
        (
            const entt::registry& registry,
            entt::entity entity,
            void* vertices,              // output array of vertices
            int verticesCount,           // number of vertices in the array
            int vertexSize,              // size of a single vertex, in bytes
            int offsetFromVertexStart    // offset from the start of each vertex to the position data, in bytes
        );
    };

} // namespace Renderer2D
} // namespace Pekan
