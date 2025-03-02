#pragma once

#include "PekanRenderer.h"

#include <string>
#include <vector>

namespace Pekan
{
namespace Renderer
{

	// A class representing an index buffer in Pekan's renderer.
	// An index buffer holds a buffer of data that can be used as indices into a vertex buffer.
	class IndexBuffer
	{
	public:

		~IndexBuffer();

		// Creates the underlying index buffer object and binds it
		void create();
		// Creates the underlying index buffer object, fills it with given data, and binds it
		void create(const void* data, long long size, BufferDataUsage dataUsage = BufferDataUsage::StaticDraw);
		// Deletes the index buffer and unbinds it.
		void destroy();

		// Fills index buffer with given data. Any previous data is removed.
		void setData(const void* data, long long size, BufferDataUsage dataUsage = BufferDataUsage::StaticDraw);

		// Checks if index buffer is valid, meaning that it has been successfully created and not yet destroyed
		inline bool isValid() const { return m_id != 0; }

		void bind() const;
		void unbind() const;

	private:

		// ID of the index buffer object
		unsigned m_id = 0;
	};

} // namespace Renderer
} // namespace Pekan