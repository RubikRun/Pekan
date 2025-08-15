#include "IndexBuffer.h"

#include "GLCall.h"

namespace Pekan
{
namespace Graphics
{
	IndexBuffer::~IndexBuffer()
	{
		PK_ASSERT(!isValid(), "You forgot to destroy() an IndexBuffer instance.", "Pekan");
	}

	void IndexBuffer::create()
	{
		PK_ASSERT(!isValid(), "Trying to create an IndexBuffer instance that is already created.", "Pekan");

		GLCall(glGenBuffers(1, &m_id));
		bind();

		m_size = 0;
	}

	void IndexBuffer::create(const void* data, long long size, BufferDataUsage dataUsage)
	{
		PK_ASSERT(!isValid(), "Trying to create an IndexBuffer instance that is already created.", "Pekan");
		PK_ASSERT(size >= 0, "Cannot create an IndexBuffer with a negative size.", "Pekan");

		GLCall(glGenBuffers(1, &m_id));
		setData(data, size, dataUsage);
	}

	void IndexBuffer::destroy()
	{
		PK_ASSERT(isValid(), "Trying to destroy an IndexBuffer instance that is not yet created.", "Pekan");

		GLCall(glDeleteBuffers(1, &m_id));
		m_id = 0;
	}

	void IndexBuffer::setData(const void* data, long long size, BufferDataUsage dataUsage)
	{
		PK_ASSERT(isValid(), "Trying to set data to an IndexBuffer that is not yet created.", "Pekan");
		PK_ASSERT(size >= 0, "Cannot set data with a negative size to an IndexBuffer.", "Pekan");

		bind();
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, RenderState::getBufferDataUsageOpenGLEnum(dataUsage)));
		m_size = size;
	}

	void IndexBuffer::setSubData(const void* data, long long offset, long long size)
	{
		PK_ASSERT(isValid(), "Trying to set subdata to an IndexBuffer that is not yet created.", "Pekan");
		PK_ASSERT(size >= 0, "Cannot set subdata with a negative size to an IndexBuffer.", "Pekan");
		PK_ASSERT(offset >= 0 && offset + size <= m_size, "Trying to set subdata that is out of range to an IndexBuffer.", "Pekan");

		bind();
		GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data));
	}

	void IndexBuffer::bind() const
	{
		PK_ASSERT(isValid(), "Trying to bind an IndexBuffer that is not yet created.", "Pekan");

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
	}

	void IndexBuffer::unbind() const
	{
		PK_ASSERT(isValid(), "Trying to unbind an IndexBuffer that is not yet created.", "Pekan");

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

} // namespace Graphics
} // namespace Pekan