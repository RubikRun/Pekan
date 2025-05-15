#include "RenderObject.h"

namespace Pekan
{
namespace Renderer
{

	void RenderObject::create
	(
		const void* vertexData,
		long long vertexDataSize,
		const VertexBufferLayout& layout,
		BufferDataUsage vertexDataUsage,
		const void* indexData,
		long long indexDataSize,
		BufferDataUsage indexDataUsage,
		const char* vertexShaderSource,
		const char* fragmentShaderSource
	)
	{
		PK_ASSERT_QUICK(vertexData != nullptr); PK_ASSERT_QUICK(vertexDataSize >= 0);
		PK_ASSERT_QUICK(indexData != nullptr); PK_ASSERT_QUICK(indexDataSize >= 0);
		PK_ASSERT_QUICK(vertexShaderSource != nullptr); PK_ASSERT_QUICK(fragmentShaderSource != nullptr);

		m_vertexArray.create();
		m_vertexBuffer.create(vertexData, vertexDataSize, vertexDataUsage);
		m_vertexArray.addVertexBuffer(m_vertexBuffer, layout);
		m_indexBuffer.create(indexData, indexDataSize, indexDataUsage);
		m_shader.create(vertexShaderSource, fragmentShaderSource);
	}

	void RenderObject::create
	(
		const void* vertexData,
		long long vertexDataSize,
		const VertexBufferLayout& layout,
		BufferDataUsage vertexDataUsage,
		const char* vertexShaderSource,
		const char* fragmentShaderSource
	)
	{
		PK_ASSERT_QUICK(vertexData != nullptr); PK_ASSERT_QUICK(vertexDataSize >= 0);
		PK_ASSERT_QUICK(vertexShaderSource != nullptr); PK_ASSERT_QUICK(fragmentShaderSource != nullptr);

		m_vertexArray.create();
		m_vertexBuffer.create(vertexData, vertexDataSize, vertexDataUsage);
		m_vertexArray.addVertexBuffer(m_vertexBuffer, layout);
		m_indexBuffer.create();
		m_shader.create(vertexShaderSource, fragmentShaderSource);
	}

	void RenderObject::create(const VertexBufferLayout& layout, const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		PK_ASSERT_QUICK(vertexShaderSource != nullptr); PK_ASSERT_QUICK(fragmentShaderSource != nullptr);

		m_vertexArray.create();
		m_vertexBuffer.create();
		m_vertexArray.addVertexBuffer(m_vertexBuffer, layout);
		m_indexBuffer.create();
		m_shader.create(vertexShaderSource, fragmentShaderSource);
	}

	void RenderObject::destroy()
	{
		PK_ASSERT_QUICK(m_shader.isValid()); PK_ASSERT_QUICK(m_indexBuffer.isValid());
		PK_ASSERT_QUICK(m_vertexBuffer.isValid()); PK_ASSERT_QUICK(m_vertexArray.isValid());

		m_shader.destroy();
		m_indexBuffer.destroy();
		m_vertexBuffer.destroy();
		m_vertexArray.destroy();
	}

	void RenderObject::bind()
	{
		PK_ASSERT_QUICK(m_vertexArray.isValid()); PK_ASSERT_QUICK(m_shader.isValid());

		m_vertexArray.bind();
		m_shader.bind();
	}

	void RenderObject::unbind()
	{
		PK_ASSERT_QUICK(m_shader.isValid()); PK_ASSERT_QUICK(m_vertexArray.isValid());

		m_shader.unbind();
		m_vertexArray.unbind();
	}

	void RenderObject::setVertexData(const void* data, long long size, BufferDataUsage dataUsage)
	{
		PK_ASSERT_QUICK(m_vertexBuffer.isValid());

		m_vertexBuffer.setData(data, size, dataUsage);
	}

	void RenderObject::setIndexData(const void* data, long long size, BufferDataUsage dataUsage)
	{
		PK_ASSERT_QUICK(m_indexBuffer.isValid());

		m_indexBuffer.setData(data, size, dataUsage);
	}

} // namespace Renderer
} // namespace Pekan