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

	bool RenderObject::isValid() const
	{
		// Assert that if vertex array is valid then all other components are valid (happens only in debug builds)
		PK_ASSERT_QUICK
		(
			!m_vertexArray.isValid() ||
			(m_vertexBuffer.isValid() || m_indexBuffer.isValid() || m_shader.isValid())
		);
		// Check only vertex array for validity in release builds
		return m_vertexArray.isValid();
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

		for (const TexturePtr& texture : m_textures)
		{
			if (texture != nullptr)
			{
				PK_ASSERT_QUICK(texture->isValid());
				texture->destroy();
			}
		}
	}

	void RenderObject::bind() const
	{
		PK_ASSERT_QUICK(m_vertexArray.isValid()); PK_ASSERT_QUICK(m_shader.isValid());

		m_vertexArray.bind();
		m_shader.bind();
		// Bind textures
		for (unsigned i = 0; i < m_textures.size(); i++)
		{
			if (m_textures[i] != nullptr)
			{
				PK_ASSERT_QUICK(m_textures[i]->isValid());
				m_textures[i]->bind(i);
			}
		}
	}

	void RenderObject::unbind() const
	{
		PK_ASSERT_QUICK(m_shader.isValid()); PK_ASSERT_QUICK(m_vertexArray.isValid());

		// Unbind textures
		for (unsigned i = 0; i < m_textures.size(); i++)
		{
			if (m_textures[i] != nullptr)
			{
				PK_ASSERT_QUICK(m_textures[i]->isValid());
				m_textures[i]->unbind(i);
			}
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

	void RenderObject::setShaderSource(const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		PK_ASSERT_QUICK(m_shader.isValid());
		m_shader.setSource(vertexShaderSource, fragmentShaderSource);
		m_textures.clear();
	}

	void RenderObject::setTextureImage(const Image& image, const char* uniformName, unsigned slot)
	{
		PK_ASSERT_QUICK(m_shader.isValid());

		// Check if requested slot is available on current hardware
		if (int(slot) >= PekanRenderer::getMaxTextureSlots())
		{
			PK_LOG_ERROR("Requested texture slot " << slot << " is not available on current hardware."
				" Maximum available texture slot is " << PekanRenderer::getMaxTextureSlots(), "Pekan");
			return;
		}

		// If textures vector is not big enough to contain requested slot, resize it
		if (slot >= m_textures.size())
		{
			m_textures.resize(slot + 1);
		}

		// Create texture with given image at the requested slot
		m_textures[slot] = std::make_shared<Texture>();
		m_textures[slot]->create(image);
		// Set slot to shader's uniform
		m_shader.bind();
		m_shader.setUniform1i(uniformName, slot);
	}

} // namespace Renderer
} // namespace Pekan