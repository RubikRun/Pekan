#include "FrameBuffer.h"

#include "GLCall.h"

namespace Pekan
{
namespace Graphics
{

	FrameBuffer::~FrameBuffer()
	{
		PK_ASSERT(!isValid(), "You forgot to destroy() a FrameBuffer instance.", "Pekan");
	}

	void FrameBuffer::create(int width, int height, int samplesPerPixel)
	{
		PK_ASSERT(!isValid(), "Trying to create a FrameBuffer instance that is already created.", "Pekan");

		m_samplesPerPixel = samplesPerPixel;
		m_width = width;
		m_height = height;

		GLCall(glGenFramebuffers(1, &m_id));
		bind();

		// Create texture,
		// either a multisample one or a normal one, depending on number of samples
		if (samplesPerPixel > 1)
		{
			createTextureMultisample();
		}
		else
		{
			createTexture();
		}
		createRenderBuffer();

		GLCall(const unsigned fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER));
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		{
			PK_LOG_ERROR("FrameBuffer failed to create with an OpenGL error: " << fboStatus, "Pekan");
		}
	}

	void FrameBuffer::destroy()
	{
		PK_ASSERT(isValid(), "Trying to destroy a FrameBuffer instance that is not yet created.", "Pekan");

		m_renderBuffer.destroy();
		if (m_samplesPerPixel > 1)
		{
			m_textureMultisample.destroy();
		}
		else
		{
			m_texture.destroy();
		}
		GLCall(glDeleteFramebuffers(1, &m_id));
		m_id = 0;
	}

	void FrameBuffer::bind() const
	{
		PK_ASSERT(isValid(), "Trying to bind a FrameBuffer that is not yet created.", "Pekan");

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
	}

	void FrameBuffer::unbind() const
	{
		PK_ASSERT(isValid(), "Trying to unbind a FrameBuffer that is not yet created.", "Pekan");

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void FrameBuffer::bindTexture() const
	{
		PK_ASSERT(isValid(), "Trying to bind a FrameBuffer's texture but FrameBuffer is not yet created.", "Pekan");

		if (m_samplesPerPixel > 1)
		{
			m_textureMultisample.bind();
		}
		else
		{
			m_texture.bind();
		}
	}

	void FrameBuffer::bindTexture(unsigned slot) const
	{
		PK_ASSERT(isValid(), "Trying to bind a FrameBuffer's texture but FrameBuffer is not yet created.", "Pekan");

		if (m_samplesPerPixel > 1)
		{
			m_textureMultisample.bind(slot);
		}
		else
		{
			m_texture.bind(slot);
		}
	}

	void FrameBuffer::resolveMultisampleToSinglesample(FrameBuffer& targetFrameBuffer)
	{
		PK_ASSERT(m_samplesPerPixel > 1, "Trying to resolve a multisample FrameBuffer to a single-sample FrameBuffer,"
			" but the FrameBuffer that's supposed to be multisample is actually single-sample.", "Pekan");
		PK_ASSERT(targetFrameBuffer.m_samplesPerPixel == 1, "Trying to resolve a multisample FrameBuffer to a single-sample FrameBuffer,"
			" but the FrameBuffer that's supposed to be single-sample is actually multisample.", "Pekan");

		GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id));
		GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetFrameBuffer.m_id));
		GLCall(glBlitFramebuffer
		(
			0, 0, m_width, m_height,
			0, 0, m_width, m_height,
			GL_COLOR_BUFFER_BIT, GL_NEAREST
		));
	}

	bool FrameBuffer::isValid() const
	{
		return m_id != 0;
	}

	void FrameBuffer::createTexture()
	{
		PK_ASSERT(isValid(), "Trying to create a texture for a FrameBuffer that is not yet created.", "Pekan");
		PK_ASSERT_QUICK(m_samplesPerPixel == 1);

		m_texture.create();
		// Set texture's size to frame buffer's width and height.
		// Make it have 3 channels - RGB, as we don't need A, we'll have a separate depth buffer.
		m_texture.setSize(m_width, m_height, 3);
		// Configure minify/magnify functions and wrap mode
		m_texture.setMinifyFunction(TextureMinifyFunction::Nearest);
		m_texture.setMagnifyFunction(TextureMagnifyFunction::Nearest);
		m_texture.setWrapModeX(TextureWrapMode::ClampToEdge);
		m_texture.setWrapModeY(TextureWrapMode::ClampToEdge);
		// Attach texture to framebuffer
		m_texture.attachToFrameBuffer(*this);
	}

	void FrameBuffer::createTextureMultisample()
	{
		PK_ASSERT(isValid(), "Trying to create a multisample texture for a FrameBuffer that is not yet created.", "Pekan");
		PK_ASSERT_QUICK(m_samplesPerPixel > 1);

		m_textureMultisample.create(m_samplesPerPixel);
		// Set texture's size to frame buffer's width and height.
		// Make it have 3 channels - RGB, as we don't need A, we'll have a separate depth buffer.
		m_textureMultisample.setSize(m_width, m_height, 3);
		// Attach texture to framebuffer
		m_textureMultisample.attachToFrameBuffer(*this);
	}

	void FrameBuffer::createRenderBuffer()
	{
		PK_ASSERT(isValid(), "Trying to create a render buffer for a FrameBuffer that is not yet created.", "Pekan");

		m_renderBuffer.create(m_width, m_height, m_samplesPerPixel);
		m_renderBuffer.attachToFrameBuffer(*this);
	}

} // namespace Graphics
} // namespace Pekan