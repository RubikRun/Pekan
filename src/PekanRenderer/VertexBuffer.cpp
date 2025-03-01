#include "VertexBuffer.h"
#define PK_FILENAME "VertexBuffer.cpp"
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
		VertexBufferDataUsage dataUsage
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

	void VertexBuffer::setData(const void* data, long long size, VertexBufferDataUsage dataUsage)
	{
		bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, getDataUsageOpenGLEnum(dataUsage));
	}

	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	unsigned VertexBuffer::getDataUsageOpenGLEnum(VertexBufferDataUsage dataUsage)
	{
		switch (dataUsage)
		{
			case VertexBufferDataUsage::StreamDraw:     return GL_STREAM_DRAW;
			case VertexBufferDataUsage::StreamRead:     return GL_STREAM_READ;
			case VertexBufferDataUsage::StreamCopy:     return GL_STREAM_COPY;
			case VertexBufferDataUsage::StaticDraw:     return GL_STATIC_DRAW;
			case VertexBufferDataUsage::StaticRead:     return GL_STATIC_READ;
			case VertexBufferDataUsage::StaticCopy:     return GL_STATIC_COPY;
			case VertexBufferDataUsage::DynamicDraw:    return GL_DYNAMIC_DRAW;
			case VertexBufferDataUsage::DynamicRead:    return GL_DYNAMIC_READ;
			case VertexBufferDataUsage::DynamicCopy:    return GL_DYNAMIC_COPY;
		}
		PK_LOG_ERRORF("Unknown VertexBufferDataUsage, cannot determine OpenGL enum.");
		return 0;
	}

} // namespace Renderer
} // namespace Pekan