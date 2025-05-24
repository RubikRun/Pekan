#include "IndexBuffer.h"

#include <glad/glad.h>

namespace Pekan
{
namespace Renderer
{
	IndexBuffer::~IndexBuffer()
	{
		if (isValid())
		{
			destroy();
		}
	}

	void IndexBuffer::create(const void* data, long long size, BufferDataUsage dataUsage)
	{
		RenderComponent::create();
		setData(data, size);
	}

	void IndexBuffer::setData(const void* data, long long size, BufferDataUsage dataUsage)
	{
		bind();
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, PekanRenderer::getBufferDataUsageOpenGLEnum(dataUsage)));
	}

	void IndexBuffer::bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
	}

	void IndexBuffer::unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	void IndexBuffer::_create()
	{
		GLCall(glGenBuffers(1, &m_id));
	}

	void IndexBuffer::_destroy()
	{
		GLCall(glDeleteBuffers(1, &m_id));
	}

} // namespace Renderer
} // namespace Pekan