#include "RenderObject.h"

namespace Demo
{
	void RenderObject::create(
		const void* vertexData,
		long long vertexDataSize,
		Pekan::Renderer::BufferDataUsage vertexDataUsage,
		const Pekan::Renderer::VertexBufferLayout& layout,
		const void* indexData,
		long long indexDataSize,
		Pekan::Renderer::BufferDataUsage indexDataUsage,
		const char* vertexShaderSource,
		const char* fragmentShaderSource
	)
	{
		m_vertexArray.create();
		m_vertexBuffer.create(vertexData, vertexDataSize, vertexDataUsage);
		m_vertexArray.addVertexBuffer(m_vertexBuffer, layout);
		m_indexBuffer.create(indexData, indexDataSize, indexDataUsage);
		m_shader.create(vertexShaderSource, fragmentShaderSource);
	}

	void RenderObject::destroy()
	{
		m_shader.destroy();
		m_indexBuffer.destroy();
		m_vertexBuffer.destroy();
		m_vertexArray.destroy();
	}

	void RenderObject::bind()
	{
		m_vertexArray.bind();
		m_shader.bind();
	}

	void RenderObject::unbind()
	{
		m_shader.unbind();
		m_vertexArray.unbind();
	}
	void RenderObject::setVertexData(const void* data, long long size, Pekan::Renderer::BufferDataUsage dataUsage)
	{
		m_vertexBuffer.setData(data, size, dataUsage);
	}
} // namespace Demo