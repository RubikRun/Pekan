#pragma once

#include "RenderState.h"

#include <string>
#include <vector>

namespace Pekan
{
namespace Graphics
{

	// An element from a layout of a vertex buffer.
	// Usually corresponds to a vertex attribute inside a shader.
	struct VertexBufferElement
	{
		// Mark class VertexBufferLayout as a friend class,
		// because it needs to set size and offset of buffer element,
		// and we want no other code to be able to set size and offset.
		friend class VertexBufferLayout;

		VertexBufferElement() = default;
		VertexBufferElement(ShaderDataType type, const std::string& name, bool normalized = false);

		// Returns number of components of the buffer element
		unsigned getComponentsCount() const;

		inline unsigned getSize() const { return m_size; }
		inline unsigned getOffset() const { return m_offset; }

		// Name of buffer element, usually corresponding to the name of the vertex attribute
		std::string name;
		// Data type of buffer element, usually corresponding to the shader type of the vertex attribute
		ShaderDataType type = ShaderDataType::None;
		// Flag for whether the corresponding vertex attribute should be normalized
		bool normalized = false;

	private:
		// Size, in bytes, of buffer element
		unsigned m_size = 0;
		// Offset, in bytes, of buffer element relative to the beginning of a vertex
		unsigned m_offset = 0;
	};

	// Layout of a vertex buffer
	class VertexBufferLayout
	{
	public:

		VertexBufferLayout() = default;
		VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements);

		// Returns stride, in bytes, between two consecutive vertices in the buffer
		unsigned getStride() const { return m_stride; }
		// Returns size of a single vertex, in bytes
		unsigned getVertexSize() const { return m_stride; }
		// Returns layout's elements
		const std::vector<VertexBufferElement>& getElements() const { return m_elements; }

		// Iterators for beginning and end of layout's elements. Useful for range-based for loop.
		std::vector<VertexBufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<VertexBufferElement>::iterator end() { return m_elements.end(); }
		std::vector<VertexBufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<VertexBufferElement>::const_iterator end() const { return m_elements.end(); }

	private: /* functions */

		// Calculates the offset of each layout element
		// and the overall stride between two consecutive vertices.
		void calculateOffsetsAndStride();

	private: /* variables */

		// Vector of layout's elements
		std::vector<VertexBufferElement> m_elements;
		// Stride, in bytes, between two consecutive vertices in the buffer, equal to the size of a single vertex.
		unsigned m_stride = 0;
	};

	// A class representing a vertex buffer on the GPU.
	// A vertex buffer holds a buffer of data that can be used as an array of vertices.
	class VertexBuffer
	{
	public:

		~VertexBuffer();

		// Creates the underlying vertex buffer object
		void create();
		// Creates the underlying vertex buffer object, fills it with given data, and binds it
		void create(const void* data, long long size, BufferDataUsage dataUsage = BufferDataUsage::StaticDraw);
		void destroy();

		// Fills vertex buffer with given data. Any previous data is removed.
		void setData(const void* data, long long size, BufferDataUsage dataUsage = BufferDataUsage::StaticDraw);
		// Fills a region of the vertex buffer with given data. Previous data in this region is overwritten.
		// @param[in] data - Data to be filled in to the region
		// @param[in] offset - Offset from the beginning of the vertex buffer to where the region begins
		// @param[in] size - Size of the region. Should match the size of given data.
		void setSubData(const void* data, long long offset, long long size);

		void bind() const;
		void unbind() const;

		// Returns size of vertex buffer's data, in bytes
		long long getSize() const { return m_size; }

		// Checks if vertex buffer contains any data
		bool hasData() const { return m_size > 0; }

		// Checks if vertex buffer is valid, meaning that it has been successfully created and not yet destroyed
		bool isValid() const { return m_id != 0; }

	private:

		// Vertex buffer's size, in bytes
		long long m_size = -1;

		// Vertex buffer's ID on the GPU
		unsigned m_id = 0;
	};

	// A binding between a vertex buffer and a layout
	struct VertexBufferBinding
	{
		VertexBufferBinding(VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
			: vertexBuffer(vertexBuffer)
			, layout(layout)
		{}

		VertexBuffer& vertexBuffer;
		VertexBufferLayout layout;
	};

} // namespace Graphics
} // namespace Pekan