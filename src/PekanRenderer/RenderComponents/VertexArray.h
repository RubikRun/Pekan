#pragma once

#include "VertexBuffer.h"
#include "RenderComponent.h"

namespace Pekan
{
namespace Renderer
{

	// A class representing a vertex array in Pekan's renderer.
	// A vertex array consists of multiple vertex buffers and maintains the state of their layouts.
	class VertexArray : public RenderComponent
	{
	public:

		~VertexArray();

		// Make base class RenderComponent's version of create() be visible in this derived class
		using RenderComponent::create;

		void bind() const override;
		void unbind() const override;

		// Adds a vertex buffer to the vertex array
		void addVertexBuffer(VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);

	private: /* functions */

		void _create() override;
		void _destroy() override;

	private: /* variables */

		// List of vertex buffer bindings associated with the vertex array
		std::vector<VertexBufferBinding> m_vertexBuffers;
	};

} // namespace Renderer
} // namespace Pekan