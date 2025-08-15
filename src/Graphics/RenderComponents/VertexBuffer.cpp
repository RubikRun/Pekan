#include "VertexBuffer.h"
#include "PekanLogger.h"

#include "GLCall.h"

namespace Pekan
{
namespace Graphics
{

	VertexBufferElement::VertexBufferElement(ShaderDataType type, const std::string& name, bool normalized)
		: name(name)
		, type(type)
		, m_size(RenderState::getShaderDataTypeSize(type))
		, m_offset(0)
		, normalized(normalized)
	{}

	unsigned VertexBufferElement::getComponentsCount() const
	{
		return RenderState::getShaderDataTypeComponentsCount(type);
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
			// The offset of each element should be equal to the accumulated size, in bytes, of all previous elements
			element.m_offset = offset;
			offset += element.m_size;
		}
		// In the end we have a sum of the sizes of all elements
		// so this is equal to the size of the whole vertex which is equal to the stride.
		m_stride = offset;
	}

	VertexBuffer::~VertexBuffer()
	{
		PK_ASSERT(!isValid(), "You forgot to destroy() a VertexBuffer instance.", "Pekan");
	}

	void VertexBuffer::create()
	{
		PK_ASSERT(!isValid(), "Trying to create a VertexBuffer instance that is already created.", "Pekan");

		GLCall(glGenBuffers(1, &m_id));
		bind();

		m_size = 0;
	}

	void VertexBuffer::create
	(
		const void* data,
		long long size,
		BufferDataUsage dataUsage
	)
	{
		PK_ASSERT(!isValid(), "Trying to create a VertexBuffer instance that is already created.", "Pekan");
		PK_ASSERT(size >= 0, "Cannot create a VertexBuffer with a negative size.", "Pekan");

		GLCall(glGenBuffers(1, &m_id));
		setData(data, size, dataUsage);
	}

	void VertexBuffer::destroy()
	{
		PK_ASSERT(isValid(), "Trying to destroy a VertexBuffer instance that is not yet created.", "Pekan");

		GLCall(glDeleteBuffers(1, &m_id));
		m_id = 0;
	}

	void VertexBuffer::setData(const void* data, long long size, BufferDataUsage dataUsage)
	{
		PK_ASSERT(isValid(), "Trying to set data to a VertexBuffer that is not yet created.", "Pekan");
		PK_ASSERT(size >= 0, "Cannot set data with a negative size to a VertexBuffer.", "Pekan");

		bind();
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, RenderState::getBufferDataUsageOpenGLEnum(dataUsage)));
		m_size = size;
	}

	void VertexBuffer::setSubData(const void* data, long long offset, long long size)
	{
		PK_ASSERT(isValid(), "Trying to set subdata to a VertexBuffer that is not yet created.", "Pekan");
		PK_ASSERT(size >= 0, "Cannot set subdata with a negative size to a VertexBuffer.", "Pekan");
		PK_ASSERT(offset >= 0 && offset + size <= m_size, "Trying to set subdata that is out of range to a VertexBuffer.", "Pekan");

		bind();
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
	}

	void VertexBuffer::bind() const
	{
		PK_ASSERT(isValid(), "Trying to bind a VertexBuffer that is not yet created.", "Pekan");

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_id));
	}

	void VertexBuffer::unbind() const
	{
		PK_ASSERT(isValid(), "Trying to unbind a VertexBuffer that is not yet created.", "Pekan");

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

} // namespace Graphics
} // namespace Pekan