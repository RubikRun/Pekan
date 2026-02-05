#pragma once

#include "RenderState.h"

namespace Pekan
{
namespace Graphics
{

	// A class representing an index buffer on the GPU.
	// An index buffer holds a buffer of data that can be used as indices into a vertex buffer.
	class IndexBuffer
	{
	public:

		~IndexBuffer();

		// Creates the underlying index buffer object
		void create();
		// Creates the underlying index buffer object, fills it with given data, and binds it
		void create(const void* data, long long size, BufferDataUsage dataUsage = BufferDataUsage::StaticDraw);
		void destroy();

		// Fills index buffer with given data. Any previous data is overwritten.
		void setData(const void* data, long long size, BufferDataUsage dataUsage = BufferDataUsage::StaticDraw);
		// Fills a region of the index buffer with given data. Previous data in this region is overwritten.
		// @param[in] data - Data to be filled in to the region
		// @param[in] offset - Offset from the beginning of the index buffer to where the region begins
		// @param[in] size - Size of the region. Should match the size of given data.
		void setSubData(const void* data, long long offset, long long size);

		void bind() const;
		void unbind() const;

		// Returns size of index buffer's data, in bytes
		long long getSize() const { return m_size; }
		// Returns the number of indices in the index buffer
		long long getCount() const { return m_size / sizeof(unsigned); }

		// Checks if index buffer contains any data
		bool hasData() const { return m_size > 0; }

		// Checks if index buffer is valid, meaning that it has been successfully created and not yet destroyed
		bool isValid() const { return m_id != 0; }

	private:

		// Index buffer's size, in bytes
		long long m_size = -1;

		// Index buffer's ID on the GPU
		unsigned m_id = 0;
	};

} // namespace Graphics
} // namespace Pekan