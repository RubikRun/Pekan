#pragma once

#include "PekanRenderer.h"

#include <string>
#include <vector>

namespace Pekan
{
namespace Renderer
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

		inline unsigned getSize() const { return size; }
		inline unsigned getOffset() const { return offset; }

		// Name of buffer element, usually corresponding to the name of the vertex attribute
		std::string name;
		// Data type of buffer element, usually corresponding to the shader type of the vertex attribute
		ShaderDataType type = ShaderDataType::None;
		// Flag for whether the corresponding vertex attribute should be normalized
		bool normalized = false;

	private:
		// Size, in bytes, of buffer element
		unsigned size = 0;
		// Offset, in bytes, of buffer element relative to the beginning of a vertex
		unsigned offset = 0;
	};

	// Layout of a vertex buffer
	class VertexBufferLayout
	{
	public:

		VertexBufferLayout() = default;
		VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements);

		inline unsigned getStride() const { return stride; }
		inline const std::vector<VertexBufferElement>& getElements() const { return elements; }

		// Iterators for beginning and end of layout's elements. Useful for range-based for loop.
		std::vector<VertexBufferElement>::iterator begin() { return elements.begin(); }
		std::vector<VertexBufferElement>::iterator end() { return elements.end(); }
		std::vector<VertexBufferElement>::const_iterator begin() const { return elements.begin(); }
		std::vector<VertexBufferElement>::const_iterator end() const { return elements.end(); }

	private: /* functions */

		// Calculates the offset of each layout element
		// and the overall stride between two consecutive vertices.
		void calculateOffsetsAndStride();

	private: /* variables */

		// Vector of layout's elements
		std::vector<VertexBufferElement> elements;
		// Stride, in bytes, between two consecutive vertices in the buffer, equal to the size of a single vertex.
		unsigned stride = 0;
	};

	// A class representing a vertex buffer in Pekan's renderer.
	// A vertex buffer simply holds a buffer of data that can be used as an array of vertices.
	class VertexBuffer
	{
	public:

		~VertexBuffer();

		// Creates the underlying vertex buffer object,
		// initializes it with given data and binds it.
		// @param data - raw data to be filled to the vertex buffer
		// @param size - size of data, in bytes
		void create(const void* data, long long size, const VertexBufferLayout& layout);
		// Deletes the vertex buffer and unbinds it.
		void destroy();

		// Checks if vertex buffer is valid, meaning that it has been successfully created and not yet destroyed
		inline bool isValid() const { return id != 0; }

		inline const VertexBufferLayout& getLayout() const { return layout; }

		void bind() const;
		void unbind() const;

	private:

		// ID of the vertex buffer object
		unsigned id = 0;

		// Layout of vertex buffer
		VertexBufferLayout layout;
	};

} // namespace Renderer
} // namespace Pekan