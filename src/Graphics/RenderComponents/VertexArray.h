#pragma once

#include "RenderState.h"
#include "VertexBuffer.h"

namespace Pekan
{
namespace Graphics
{

	// A class representing a vertex array on the GPU.
	// A vertex array consists of multiple vertex buffers and maintains the state of their layouts.
	class VertexArray
	{
	public:

		~VertexArray();

		// Creates the underlying vertex array object
		void create();
		void destroy();

		void bind() const;
		void unbind() const;

		// Adds a vertex buffer to the vertex array
		void addVertexBuffer(VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);

		// Checks if vertex array is valid, meaning that it has been successfully created and not yet destroyed
		bool isValid() const { return m_id != 0; }

	private: /* variables */

		// List of vertex buffer bindings associated with the vertex array
		std::vector<VertexBufferBinding> m_vertexBuffers;

		// Vertex array's ID on the GPU
		unsigned m_id = 0;
	};

} // namespace Graphics
} // namespace Pekan