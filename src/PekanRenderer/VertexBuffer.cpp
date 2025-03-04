#include "VertexBuffer.h"
#include "Logger/PekanLogger.h"

#include <glad/glad.h>

namespace Pekan
{
namespace Renderer
{

	VertexBufferElement::VertexBufferElement(ShaderDataType type, const std::string& name, bool normalized)
		: name(name)
		, type(type)
		, m_size(PekanRenderer::getShaderDataTypeSize(type))
		, m_offset(0)
		, normalized(normalized)
	{}

	unsigned VertexBufferElement::getComponentsCount() const
	{
		return PekanRenderer::getShaderDataTypeComponentsCount(type);
	}

	VertexBufferLayout::VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements)
		: m_elements(elements)
	{
		calculateOffsetsAndStride();
	}

	void VertexBufferLayout::calculateOffsetsAndStride()
	{
		unsigned offset = 0;
		// Loop over layout's elements and calculate each element's offset
		for (auto& element : m_elements)
		{
			// The offset of each element should be equal to the summed size, in bytes, of all previous elements
			element.m_offset = offset;
			offset += element.m_size;
		}
		// In the end we have a sum of the sizes of all elements
		// so this is equal to the size of the whole vertex which is equal to the stride.
		m_stride = offset;
	}

	VertexBuffer::~VertexBuffer()
	{
		if (m_id != 0)
		{
			destroy();
		}
	}

	void VertexBuffer::create()
	{
		glGenBuffers(1, &m_id);
		bind();
	}

	void VertexBuffer::create(
		const void* data,
		long long size,
		BufferDataUsage dataUsage
	)
	{
		create();
		setData(data, size, dataUsage);
	}

	void VertexBuffer::destroy()
	{
		unbind();
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}

	void VertexBuffer::setData(const void* data, long long size, BufferDataUsage dataUsage)
	{
		bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, PekanRenderer::getBufferDataUsageOpenGLEnum(dataUsage));
	}

	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

} // namespace Renderer
} // namespace Pekan