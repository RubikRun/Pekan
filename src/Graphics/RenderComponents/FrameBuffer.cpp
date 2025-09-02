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

	void FrameBuffer::create(int width, int height)
	{
		PK_ASSERT(!isValid(), "Trying to create a FrameBuffer instance that is already created.", "Pekan");

		GLCall(glGenFramebuffers(1, &m_id));
		bind();

		createTexture(width, height);
		createRenderBuffer(width, height);

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
		m_texture.destroy();
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

		m_texture.bind();
	}

	bool FrameBuffer::isValid() const
	{
		return m_id != 0;
	}

	void FrameBuffer::createTexture(int width, int height)
	{
		PK_ASSERT(isValid(), "Trying to create a texture for a FrameBuffer that is not yet created.", "Pekan");

		m_texture.create();
		// Set texture's size to given width and height.
		// Make it have 3 channels - RGB, as we don't need A, we'll have a separate depth buffer.
		m_texture.setSize(width, height, 3);
		// Configure minify/magnify functions and wrap mode
		m_texture.setMinifyFunction(TextureMinifyFunction::Linear);
		m_texture.setMagnifyFunction(TextureMagnifyFunction::Linear);
		m_texture.setWrapModeX(TextureWrapMode::ClampToEdge);
		m_texture.setWrapModeY(TextureWrapMode::ClampToEdge);
		// Attach texture to framebuffer
		m_texture.attachToFrameBuffer(*this);
	}

	void FrameBuffer::createRenderBuffer(int width, int height)
	{
		PK_ASSERT(isValid(), "Trying to create a render buffer for a FrameBuffer that is not yet created.", "Pekan");

		m_renderBuffer.create(width, height);
		m_renderBuffer.attachToFrameBuffer(*this);
	}

} // namespace Graphics
} // namespace Pekan