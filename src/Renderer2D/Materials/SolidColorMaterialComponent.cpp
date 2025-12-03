#pragma once

#include <glm/glm.hpp>
#include "SolidColorMaterialComponent.h"

namespace Pekan
{
namespace Renderer2D
{

	void SolidColorMaterialComponent::fillVerticesColorAttribute(void* vertices, int verticesCount, int vertexSize, int colorAttributeOffset)
	{

		/*
		// Get entity's material component
		const SolidColorMaterialComponent& material = registry.get<SolidColorMaterialComponent>(entity);
		// Set vertex colors
		VerticesAttributeView colorAttrView{ vertices, verticesCount, vertexSize, colorAttributeOffset };
		for (int i = 0; i < verticesCount; i++)
		{
			colorAttrView.setVertexAttribute<glm::vec4>(i, material.color);
		}
		*/
	}

} // namespace Renderer2D
} // namespace Pekan
