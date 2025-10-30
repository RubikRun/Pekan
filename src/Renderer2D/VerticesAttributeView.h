#pragma once

#include "PekanLogger.h"

namespace Pekan
{
namespace Renderer2D
{

    // Structure defining a view of a vertex attribute inside a vertices array
    struct VerticesAttributeView
    {
        void* vertices = nullptr;	            // pointer to an array of vertices
        const int verticesCount = 0;            // number of vertices in the array
        const int vertexSize = 0;	            // size of a single vertex, in bytes
        const int offsetFromVertexStart = 0;    // offset from the start of each vertex to a the attribute, in bytes

        // Sets the attribute of a given vertex to a given value
        template <typename T>
        void setVertexAttribute
        (
            int vertexIndex,    // index of the vertex whose attribute to set
            T attrValue         // attribute value to set
        );

        // Gets the attribute of a given vertex
        template <typename T>
        T getVertexAttribute
        (
            int vertexIndex    // index of the vertex whose attribute to get
        );
    };

    template <typename T>
    void VerticesAttributeView::setVertexAttribute(int vertexIndex, T attrValue)
    {
		PK_ASSERT(vertices != nullptr, "Pointer to vertices data is null.", "Pekan");
		PK_ASSERT(vertexIndex >= 0 && vertexIndex < verticesCount, "Given vertex index is out of bounds.", "Pekan");

        char* vertexPtr = static_cast<char*>(vertices) + vertexIndex * vertexSize;
        *reinterpret_cast<T*>(vertexPtr + offsetFromVertexStart) = attrValue;
    }

    template <typename T>
    T VerticesAttributeView::getVertexAttribute(int vertexIndex)
    {
        PK_ASSERT(vertices != nullptr, "Pointer to vertices data is null.", "Pekan");
        PK_ASSERT(vertexIndex >= 0 && vertexIndex < verticesCount, "Given vertex index is out of bounds.", "Pekan");
        char* vertexPtr = static_cast<char*>(vertices) + vertexIndex * vertexSize;
        return *reinterpret_cast<T*>(vertexPtr + offsetFromVertexStart);
    }

} // namespace Renderer2D
} // namespace Pekan
