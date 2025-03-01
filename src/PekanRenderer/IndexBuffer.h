#pragma once

#include "PekanRenderer.h"

#include <string>
#include <vector>

namespace Pekan
{
namespace Renderer
{

	// A class representing an index buffer in Pekan's renderer.
	// An index buffer holds a buffer of data that can be used as indices into a vertex array.
	class IndexBuffer
	{
	public:

		~IndexBuffer();

		// Creates the underlying index buffer object,
		// initializes it with given data and binds it.
		// @param data - raw data to be filled to the index buffer
		// @param size - size of data, in bytes
		void create(const void* data, long long size);
		// Deletes the index buffer and unbinds it.
		void destroy();

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