#include "IndexBuffer.h"

#include <glad/glad.h>

namespace Pekan
{
namespace Renderer
{

	IndexBuffer::~IndexBuffer()
	{
		if (m_id != 0)
		{
			destroy();
		}
	}

	void IndexBuffer::create()
	{
		glGenBuffers(1, &m_id);
		bind();
	}

	void IndexBuffer::create(const void* data, long long size, BufferDataUsage dataUsage)
	{
		create();
		setData(data, size);
	}

	void IndexBuffer::destroy()
	{
		unbind();
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}

	void IndexBuffer::setData(const void* data, long long size, BufferDataUsage dataUsage)
	{
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, PekanRenderer::getBufferDataUsageOpenGLEnum(dataUsage));
	}

	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

} // namespace Renderer
} // namespace Pekan