#include "VertexBuffer.h"

#include <glad/glad.h>

namespace Pekan
{
namespace Renderer
{

	VertexBufferElement::VertexBufferElement(ShaderDataType type, const std::string& name, bool normalized)
		: name(name)
		, type(type)
		, size(PekanRenderer::getShaderDataTypeSize(type))
		, offset(0)
		, normalized(normalized)
	{}

	unsigned VertexBufferElement::getComponentsCount() const
	{
		return PekanRenderer::getShaderDataTypeComponentsCount(type);
	}

	VertexBufferLayout::VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements)
		: elements(elements)
	{
		calculateOffsetsAndStride();
	}

	void VertexBufferLayout::calculateOffsetsAndStride()
	{
		unsigned offset = 0;
		// Loop over layout's elements and calculate each element's offset
		for (auto& element : elements)
		{
			// The offset of each element should be equal to the summed size, in bytes, of all previous elements
			element.offset = offset;
			offset += element.size;
		}
		// In the end we have a sum of the sizes of all elements
		// so this is equal to the size of the whole vertex which is equal to the stride.
		stride = offset;
	}

	VertexBuffer::~VertexBuffer()
	{
		if (id != 0)
		{
			destroy();
		}
	}

	void VertexBuffer::create(const void* data, long long size, const VertexBufferLayout& layout)
	{
		glGenBuffers(1, &id);
		bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		this->layout = layout;
	}

	void VertexBuffer::destroy()
	{
		unbind();
		glDeleteBuffers(1, &id);
		id = 0;
	}

	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	void VertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

} // namespace Renderer
} // namespace Pekan