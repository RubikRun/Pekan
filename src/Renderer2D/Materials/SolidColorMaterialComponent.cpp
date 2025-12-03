#pragma once

#include <glm/glm.hpp>
#include "SolidColorMaterialComponent.h"

#include "VerticesAttributeView.h"

namespace Pekan
{
namespace Renderer2D
{

    void SolidColorMaterialComponent::getVertexColors(void* vertices, int verticesCount, int vertexSize, int colorAttributeOffset) const
    {
        VerticesAttributeView colorAttrView{ vertices, verticesCount, vertexSize, colorAttributeOffset };
        for (int i = 0; i < verticesCount; i++)
        {
            colorAttrView.setVertexAttribute<glm::vec4>(i, color);
        }
    }

} // namespace Renderer2D
} // namespace Pekan
