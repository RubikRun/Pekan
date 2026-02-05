#include "DrawObject.h"

#include "PekanLogger.h"

namespace Pekan
{
namespace Graphics
{

	constexpr BufferDataUsage DEFAULT_VERTEX_DATA_USAGE = BufferDataUsage::DynamicDraw;
	constexpr BufferDataUsage DEFAULT_INDEX_DATA_USAGE = BufferDataUsage::DynamicDraw;

	void DrawObject::create
	(
		const void* vertexData,
		long long vertexDataSize,
		const VertexBufferLayout& layout,
		BufferDataUsage vertexDataUsage,
		const char* vertexShaderSource,
		const char* fragmentShaderSource
	)
	{
		PK_ASSERT(!isValid(), "Trying to create a DrawObject instance that is not yet created.", "Pekan");
		PK_ASSERT(vertexDataSize >= 0, "Trying to create a DrawObject with negative vertex data size.", "Pekan");
		PK_ASSERT(vertexShaderSource != nullptr, "Trying to create a DrawObject with null vertex shader source.", "Pekan");
		PK_ASSERT(fragmentShaderSource != nullptr, "Trying to create a DrawObject with null fragment shader source.", "Pekan");

		m_vertexDataUsage = vertexDataUsage;
		m_vertexBufferLayout = layout;

		m_vertexArray.create();
		m_vertexBuffer.create(vertexData, vertexDataSize, vertexDataUsage);
		m_vertexArray.addVertexBuffer(m_vertexBuffer, layout);
		m_indexBuffer.create();
		m_shader.create(vertexShaderSource, fragmentShaderSource);

		m_isValid = true;
	}

	void DrawObject::create(const VertexBufferLayout& layout, const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		PK_ASSERT(!isValid(), "Trying to create a DrawObject instance that is not yet created.", "Pekan");
		PK_ASSERT(vertexShaderSource != nullptr, "Trying to create a DrawObject with null vertex shader source.", "Pekan");
		PK_ASSERT(fragmentShaderSource != nullptr, "Trying to create a DrawObject with null fragment shader source.", "Pekan");

		m_vertexBufferLayout = layout;

		m_vertexArray.create();
		m_vertexBuffer.create();
		m_vertexArray.addVertexBuffer(m_vertexBuffer, layout);
		m_indexBuffer.create();
		m_shader.create(vertexShaderSource, fragmentShaderSource);

		m_isValid = true;
	}

	void DrawObject::destroy()
	{
		PK_ASSERT(isValid(), "Trying to destroy a DrawObject that is not yet created.", "Pekan");

		m_vertexDataUsage = BufferDataUsage::None;
		m_indexDataUsage = BufferDataUsage::None;

		m_shader.destroy();
		m_indexBuffer.destroy();
		m_vertexBuffer.destroy();
		m_vertexArray.destroy();

		for (const Texture2D_Ptr& texture : m_textures)
		{
			if (texture != nullptr)
			{
				texture->destroy();
			}
		}

		m_isValid = false;
	}

	void DrawObject::render(DrawMode mode) const
	{
		bind();

		if (m_indexBuffer.hasData())
		{
			const unsigned indicesCount = unsigned(m_indexBuffer.getCount());
			RenderCommands::drawIndexed(indicesCount, mode);
		}
		else
		{
			const unsigned verticesCount = unsigned(m_vertexBuffer.getSize()) / m_vertexBufferLayout.getVertexSize();
			RenderCommands::draw(verticesCount, mode);
		}
	}

	void DrawObject::setVertexData(const void* data, long long size)
	{
		PK_ASSERT(isValid(), "Trying to set vertex data to a DrawObject that is not yet created.", "Pekan");

		if (m_vertexDataUsage == BufferDataUsage::None)
		{
			m_vertexDataUsage = DEFAULT_VERTEX_DATA_USAGE;
		}

		m_vertexArray.bind();
		m_vertexBuffer.setData(data, size, m_vertexDataUsage);
	}

	void DrawObject::setVertexData(const void* data, long long size, BufferDataUsage dataUsage)
	{
		PK_ASSERT(isValid(), "Trying to set vertex data to a DrawObject that is not yet created.", "Pekan");

		m_vertexArray.bind();
		m_vertexBuffer.setData(data, size, dataUsage);
		m_vertexDataUsage = dataUsage;
	}

	void DrawObject::setVertexSubData(const void* data, long long offset, long long size)
	{
		PK_ASSERT(isValid(), "Trying to set vertex subdata to a DrawObject that is not yet created.", "Pekan");

		m_vertexArray.bind();
		m_vertexBuffer.setSubData(data, offset, size);
	}

	void DrawObject::setIndexData(const void* data, long long size)
	{
		PK_ASSERT(isValid(), "Trying to set index data to a DrawObject that is not yet created.", "Pekan");

		if (m_indexDataUsage == BufferDataUsage::None)
		{
			m_indexDataUsage = DEFAULT_INDEX_DATA_USAGE;
		}

		m_vertexArray.bind();
		m_indexBuffer.setData(data, size, m_indexDataUsage);
	}

	void DrawObject::setIndexData(const void* data, long long size, BufferDataUsage dataUsage)
	{
		PK_ASSERT(isValid(), "Trying to set index data to a DrawObject that is not yet created.", "Pekan");

		m_vertexArray.bind();
		m_indexBuffer.setData(data, size, dataUsage);
		m_indexDataUsage = dataUsage;
	}

	void DrawObject::setIndexSubData(const void* data, long long offset, long long size)
	{
		PK_ASSERT(isValid(), "Trying to set index subdata to a DrawObject that is not yet created.", "Pekan");

		m_vertexArray.bind();
		m_indexBuffer.setSubData(data, offset, size);
	}

	void DrawObject::setShaderSource(const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		PK_ASSERT(isValid(), "Trying to set shader source to a DrawObject that is not yet created.", "Pekan");

		m_shader.setSource(vertexShaderSource, fragmentShaderSource);
		clearTextures();
	}

	void DrawObject::setTextureImage(const Image& image, const char* uniformName, unsigned slot)
	{
		PK_ASSERT(isValid(), "Trying to set texture image to a DrawObject that is not yet created.", "Pekan");

		// Check if requested slot is available on current hardware
		if (int(slot) >= RenderState::getMaxTextureSlots())
		{
			PK_LOG_ERROR("Requested texture slot " << slot << " is not available on current hardware."
				" Maximum available texture slot is " << RenderState::getMaxTextureSlots() - 1, "Pekan");
			return;
		}

		// If textures vector is not big enough to contain requested slot, resize it
		if (slot >= m_textures.size())
		{
			m_textures.resize(slot + 1);
		}

		// Create texture with given image at the requested slot
		m_textures[slot] = std::make_shared<Texture2D>();
		m_textures[slot]->create(image);
		// Set shader's slot uniform to the given slot
		m_shader.bind();
		m_shader.setUniform1i(uniformName, slot);
	}

	bool DrawObject::isValid() const
	{
		// Assert that validity of DrawObject is equivalent to validity of all components
		PK_DEBUG_CODE
		(
			if (m_isValid)
			{
				PK_ASSERT_QUICK(m_vertexArray.isValid()); PK_ASSERT_QUICK(m_vertexBuffer.isValid());
				PK_ASSERT_QUICK(m_indexBuffer.isValid()); PK_ASSERT_QUICK(m_shader.isValid());
			}
			else
			{
				PK_ASSERT_QUICK(!m_vertexArray.isValid()); PK_ASSERT_QUICK(!m_vertexBuffer.isValid());
				PK_ASSERT_QUICK(!m_indexBuffer.isValid()); PK_ASSERT_QUICK(!m_shader.isValid());
			}
				);

		return m_isValid;
	}

	void DrawObject::clearTextures()
	{
		// First manually destroy each texture
		for (unsigned i = 0; i < m_textures.size(); i++)
		{
			if (m_textures[i] != nullptr)
			{
				PK_ASSERT(m_textures[i]->isValid(), "There is an invalid texture in a DrawObject.", "Pekan");
				m_textures[i]->destroy();
			}
		}
		// Then clear list of texture pointers
		m_textures.clear();
	}

	void DrawObject::bind() const
	{
		PK_ASSERT(isValid(), "Trying to bind a DrawObject that is not yet created.", "DrawObject");

		m_vertexArray.bind();
		m_shader.bind();
		// Bind textures
		for (unsigned i = 0; i < m_textures.size(); i++)
		{
			PK_ASSERT(m_textures[i] != nullptr, "There is a null texture in a DrawObject.", "Pekan");
			if (m_textures[i] != nullptr)
			{
				PK_ASSERT(m_textures[i]->isValid(), "There is an invalid texture in a DrawObject.", "Pekan");
				m_textures[i]->bind(i);
			}
		}
	}

	void DrawObject::unbind() const
	{
		PK_ASSERT(isValid(), "Trying to unbind a DrawObject that is not yet created.", "DrawObject");

		// Unbind textures
		for (unsigned i = 0; i < m_textures.size(); i++)
		{
			PK_ASSERT(m_textures[i] != nullptr, "There is a null texture in a DrawObject.", "Pekan");
			if (m_textures[i] != nullptr)
			{
				PK_ASSERT(m_textures[i]->isValid(), "There is an invalid texture in a DrawObject.", "Pekan");
				m_textures[i]->unbind(i);
			}
		}
		m_shader.unbind();
		m_vertexArray.unbind();
	}

} // namespace Graphics
} // namespace Pekan