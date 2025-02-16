#pragma once

namespace Pekan
{
namespace Renderer
{

	// A class representing a vertex buffer in Pekan's renderer.
	// A vertex buffer simply holds a buffer of data that can be used as an array of vertices.
	class VertexBuffer
	{
	public:
		~VertexBuffer();

		// Creates the underlying vertex buffer object,
		// initializes it with given data and binds it.
		// @param data - raw data to be filled to the vertex buffer
		// @param size - size of data in bytes
		void create(const void* data, long long size);
		// Deletes the vertex buffer and unbinds it.
		void destroy();

		void bind() const;
		void unbind() const;

	private:
		// ID of the vertex buffer object
		unsigned id = 0;
	};

} // namespace Renderer
} // namespace Pekan