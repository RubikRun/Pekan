#include "Texture2DMultisample.h"

#include "PekanLogger.h"
#include "GLCall.h"
#include "Image.h"
#include "FrameBuffer.h"

#include <glm/glm.hpp>

static const unsigned DEFAULT_PIXEL_TYPE = GL_UNSIGNED_BYTE;

namespace Pekan {
namespace Graphics {

	Texture2DMultisample::~Texture2DMultisample()
	{
		PK_ASSERT(!isValid(), "You forgot to destroy() a Texture2DMultisample instance.", "Pekan");
	}

	void Texture2DMultisample::create(int samplesPerTexel)
	{
		PK_ASSERT(!isValid(), "Trying to create a Texture2DMultisample instance that is already created.", "Pekan");
		PK_ASSERT(samplesPerTexel > 1, "Trying to create a Texture2DMultisample with samples per texel <= 1.", "Pekan");

		m_samplesPerTexel = samplesPerTexel;

		GLCall(glGenTextures(1, &m_id));
		bind();
	}

	void Texture2DMultisample::destroy()
	{
		PK_ASSERT(isValid(), "Trying to destroy a Texture2DMultisample instance that is not yet created.", "Pekan");

		GLCall(glDeleteTextures(1, &m_id));
		m_id = 0;
	}

	void Texture2DMultisample::setSize(int width, int height, int numChannels)
	{
		PK_ASSERT(isValid(), "Trying to set size of a Texture2DMultisample that is not yet created.", "Pekan");

		bind();

		// Establish texture's data storage with given size
		unsigned format = 0, internalFormat = 0;
		getFormat(numChannels, format, internalFormat);
		GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samplesPerTexel, internalFormat, width, height, GL_TRUE));
	}

	void Texture2DMultisample::bind() const
	{
		PK_ASSERT(isValid(), "Trying to bind a Texture2DMultisample that is not yet created.", "Pekan");

		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_id));
	}

	void Texture2DMultisample::unbind() const
	{
		PK_ASSERT(isValid(), "Trying to unbind a Texture2DMultisample that is not yet created.", "Pekan");

		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
	}

	void Texture2DMultisample::bind(unsigned slot) const
	{
		PK_ASSERT(isValid(), "Trying to bind a Texture2DMultisample that is not yet created to slot " << slot << ".", "Pekan");

		activateSlot(slot);
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_id));
	}

	void Texture2DMultisample::unbind(unsigned slot) const
	{
		PK_ASSERT(isValid(), "Trying to unbind a Texture2DMultisample that is not yet created from slot " << slot << ".", "Pekan");

		activateSlot(slot);
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
	}

	void Texture2DMultisample::activateSlot(unsigned slot)
	{
		const unsigned slotEnumValue = RenderState::getTextureSlotOpenGLEnum(slot);
		GLCall(glActiveTexture(slotEnumValue));
	}

	void Texture2DMultisample::attachToFrameBuffer(const FrameBuffer& frameBuffer) const
	{
		frameBuffer.bind();
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_id, 0));
	}

	void Texture2DMultisample::getFormat(int numChannels, unsigned& format, unsigned& internalFormat)
	{
		PK_ASSERT_QUICK(numChannels >= 0);

		switch (numChannels)
		{
			case 1:    format = GL_RED;     internalFormat = GL_R8;       break;
			case 2:    format = GL_RG;      internalFormat = GL_RG8;      break;
			case 3:    format = GL_RGB;     internalFormat = GL_RGB8;     break;
			case 4:    format = GL_RGBA;    internalFormat = GL_RGBA8;    break;
			default: PK_LOG_ERROR("Trying to get texture format for an unsupported number of channels.", "Pekan"); break;
		}
	}

} // namespace Pekan
} // namespace Graphics