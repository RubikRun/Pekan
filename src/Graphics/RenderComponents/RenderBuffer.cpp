#include "RenderBuffer.h"

#include "GLCall.h"
#include "FrameBuffer.h"

namespace Pekan
{
namespace Graphics
{

	RenderBuffer::~RenderBuffer()
	{
		PK_ASSERT(!isValid(), "You forgot to destroy() a RenderBuffer instance.", "Pekan");
	}

	void RenderBuffer::create(int width, int height, int samplesPerPixel)
	{
		PK_ASSERT(!isValid(), "Trying to create a RenderBuffer instance that is already created.", "Pekan");
		PK_ASSERT(samplesPerPixel > 0, "Trying to create a RenderBuffer with samples per pixel <= 0", "Pekan");

		GLCall(glGenRenderbuffers(1, &m_id));
		bind();
		
		if (samplesPerPixel > 1)
		{
			GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, samplesPerPixel, GL_DEPTH24_STENCIL8, width, height));

		}
		else
		{
			GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
		}
	}

	void RenderBuffer::destroy()
	{
		PK_ASSERT(isValid(), "Trying to destroy a RenderBuffer instance that is not yet created.", "Pekan");

		GLCall(glDeleteRenderbuffers(1, &m_id));
		m_id = 0;
	}

	void RenderBuffer::bind() const
	{
		PK_ASSERT(isValid(), "Trying to bind a RenderBuffer that is not yet created.", "Pekan");

		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_id));
	}

	void RenderBuffer::unbind() const
	{
		PK_ASSERT(isValid(), "Trying to unbind a RenderBuffer that is not yet created.", "Pekan");

		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	}

	void RenderBuffer::attachToFrameBuffer(const FrameBuffer& frameBuffer) const
	{
		frameBuffer.bind();
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_id));
	}

} // namespace Graphics
} // namespace Pekan