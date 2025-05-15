#include "RenderObject.h"

namespace Pekan
{
namespace Renderer
{

	static const BufferDataUsage DEFAULT_VERTEX_DATA_USAGE = BufferDataUsage::DynamicDraw;
	static const BufferDataUsage DEFAULT_INDEX_DATA_USAGE = BufferDataUsage::DynamicDraw;

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

		m_vertexDataUsage = vertexDataUsage;
		m_indexDataUsage = indexDataUsage;

		m_vertexArray.create();
		m_vertexBuffer.create(vertexData, vertexDataSize, vertexDataUsage);
		m_vertexArray.addVertexBuffer(m_vertexBuffer, layout);
		m_indexBuffer.create(indexData, indexDataSize, indexDataUsage);
		m_shader.create(vertexShaderSource, fragmentShaderSource);
		m_texture.create();
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

		m_vertexDataUsage = vertexDataUsage;

		m_vertexArray.create();
		m_vertexBuffer.create(vertexData, vertexDataSize, vertexDataUsage);
		m_vertexArray.addVertexBuffer(m_vertexBuffer, layout);
		m_indexBuffer.create();
		m_shader.create(vertexShaderSource, fragmentShaderSource);
		m_texture.create();
	}

	void RenderObject::create(const VertexBufferLayout& layout, const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		PK_ASSERT_QUICK(vertexShaderSource != nullptr); PK_ASSERT_QUICK(fragmentShaderSource != nullptr);

		m_vertexArray.create();
		m_vertexBuffer.create();
		m_vertexArray.addVertexBuffer(m_vertexBuffer, layout);
		m_indexBuffer.create();
		m_shader.create(vertexShaderSource, fragmentShaderSource);
		m_texture.create();
	}

	void RenderObject::destroy()
	{
		PK_ASSERT_QUICK(m_shader.isValid()); PK_ASSERT_QUICK(m_indexBuffer.isValid());
		PK_ASSERT_QUICK(m_vertexBuffer.isValid()); PK_ASSERT_QUICK(m_vertexArray.isValid());

		m_vertexDataUsage = BufferDataUsage::None;
		m_indexDataUsage = BufferDataUsage::None;

		m_shader.destroy();
		m_indexBuffer.destroy();
		m_vertexBuffer.destroy();
		m_vertexArray.destroy();
		m_texture.destroy();
	}

	void RenderObject::bind()
	{
		PK_ASSERT_QUICK(m_vertexArray.isValid()); PK_ASSERT_QUICK(m_shader.isValid()); PK_ASSERT_QUICK(m_texture.isValid());

		m_vertexArray.bind();
		m_shader.bind();
		if (m_textureSlot != 0xffffffff)
		{
			m_texture.bind(m_textureSlot);
		}
	}

	void RenderObject::unbind()
	{
		PK_ASSERT_QUICK(m_shader.isValid()); PK_ASSERT_QUICK(m_vertexArray.isValid()); PK_ASSERT_QUICK(m_texture.isValid());

		if (m_textureSlot != 0xffffffff)
		{
			m_texture.bind(m_textureSlot);
		}
		m_shader.unbind();
		m_vertexArray.unbind();
	}

	void RenderObject::setVertexData(const void* data, long long size)
	{
		PK_ASSERT_QUICK(m_vertexBuffer.isValid());

		if (m_vertexDataUsage == BufferDataUsage::None)
		{
			m_vertexDataUsage = DEFAULT_VERTEX_DATA_USAGE;
		}

		m_vertexBuffer.setData(data, size, m_vertexDataUsage);
	}

	void RenderObject::setVertexData(const void* data, long long size, BufferDataUsage dataUsage)
	{
		PK_ASSERT_QUICK(m_vertexBuffer.isValid());

		m_vertexBuffer.setData(data, size, dataUsage);
		m_vertexDataUsage = dataUsage;
	}

	void RenderObject::setIndexData(const void* data, long long size)
	{
		PK_ASSERT_QUICK(m_indexBuffer.isValid());

		if (m_indexDataUsage == BufferDataUsage::None)
		{
			m_indexDataUsage = DEFAULT_INDEX_DATA_USAGE;
		}

		m_indexBuffer.setData(data, size, m_indexDataUsage);
	}

	void RenderObject::setIndexData(const void* data, long long size, BufferDataUsage dataUsage)
	{
		PK_ASSERT_QUICK(m_indexBuffer.isValid());

		m_indexBuffer.setData(data, size, dataUsage);
		m_indexDataUsage = dataUsage;
	}

	void RenderObject::setTextureImage(const Image& image, const char* uniformName, unsigned slot)
	{
		PK_ASSERT_QUICK(m_shader.isValid()); PK_ASSERT_QUICK(m_texture.isValid());

		m_texture.setImage(image);
		m_shader.setUniform1i(uniformName, slot);
		m_textureSlot = slot;
	}

} // namespace Renderer
} // namespace Pekan