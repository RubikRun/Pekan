#pragma once

#include "VertexBuffer.h"

namespace Pekan
{
namespace Renderer
{

	// A class representing a vertex array in Pekan's renderer.
	// A vertex array consists of multiple vertex buffers and maintains the state of their layouts.
	class VertexArray
	{
	public:

		~VertexArray();

		// Creates the underlying vertex array object and binds it
		void create();
		// Deletes the vertex array and unbinds it
		void destroy();

		// Checks if vertex array is valid, meaning that it has been successfully created and not yet destroyed
		inline bool isValid() { return id != 0; }

		void bind() const;
		void unbind() const;

		// Adds a vertex buffer to the vertex array
		void addVertexBuffer(VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);

	private:

		// List of vertex buffer bindings associated with the vertex array
		std::vector<VertexBufferBinding> vertexBuffers;
		
		// ID of the vertex array object
		unsigned id = 0;
	};

} // namespace Renderer
} // namespace Pekan