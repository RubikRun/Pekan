#include "Texture2D.h"

#include "PekanLogger.h"
#include "GLCall.h"
#include "Image.h"

#include <glm/glm.hpp>

static const unsigned DEFAULT_PIXEL_TYPE = GL_UNSIGNED_BYTE;

namespace Pekan {
namespace Graphics {

	static const TextureMinifyFunction DEFAULT_TEXTURE_MINIFY_FUNCTION = TextureMinifyFunction::LinearOnLinearMipmap;
	static const TextureMagnifyFunction DEFAULT_TEXTURE_MAGNIFY_FUNCTION = TextureMagnifyFunction::Linear;
	static const TextureWrapMode DEFAULT_WRAP_MODE_X = TextureWrapMode::ClampToBorder;
	static const TextureWrapMode DEFAULT_WRAP_MODE_Y = TextureWrapMode::ClampToBorder;
	static const glm::vec4 DEFAULT_BORDER_COLOR = glm::vec4(255, 0, 0, 255);

	Texture2D::~Texture2D()
	{
		if (isValid())
		{
			destroy();
		}
	}

	void Texture2D::create()
	{
		RenderComponent::create();

		// Configure the default minify and magnify functions of the texture
		setMinifyFunction(DEFAULT_TEXTURE_MINIFY_FUNCTION);
		setMagnifyFunction(DEFAULT_TEXTURE_MAGNIFY_FUNCTION);

		// Configure texture wrapping
		setWrapModeX(DEFAULT_WRAP_MODE_X);
		setWrapModeY(DEFAULT_WRAP_MODE_Y);
		setBorderColor(DEFAULT_BORDER_COLOR);
	}

	void Texture2D::create(const Image& image)
	{
		create();
		setImage(image);
	}

	void Texture2D::setImage(const Image& image)
	{
		bind();

		// Set image data to the texture object
		unsigned format = 0, internalFormat = 0;
		getFormat(image, format, internalFormat);
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.getWidth(), image.getHeight(), 0, format, DEFAULT_PIXEL_TYPE, image.getData()));
		// Generate mipmaps
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}

	void Texture2D::bind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, m_id));
	}

	void Texture2D::unbind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void Texture2D::bind(unsigned slot) const
	{
		activateSlot(slot);
		GLCall(glBindTexture(GL_TEXTURE_2D, m_id));
	}

	void Texture2D::unbind(unsigned slot) const
	{
		activateSlot(slot);
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void Texture2D::activateSlot(unsigned slot)
	{
		const unsigned slotEnumValue = RenderState::getTextureSlotOpenGLEnum(slot);
		GLCall(glActiveTexture(slotEnumValue));
	}

	void Texture2D::setMinifyFunction(TextureMinifyFunction function)
	{
		bind();
		GLCall(glTexParameteri
		(
			GL_TEXTURE_2D, 
			GL_TEXTURE_MIN_FILTER,
			RenderState::getTextureMinifyFunctionOpenGLEnum(function)
		));
	}

	void Texture2D::setMagnifyFunction(TextureMagnifyFunction function)
	{
		bind();
		GLCall(glTexParameteri
		(
			GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER,
			RenderState::getTextureMagnifyFunctionOpenGLEnum(function)
		));
	}

	void Texture2D::setWrapModeX(TextureWrapMode wrapMode)
	{
		bind();
		GLCall(glTexParameteri
		(
			GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_S,
			RenderState::getTextureWrapModeOpenGLEnum(wrapMode)
		));
	}

	void Texture2D::setWrapModeY(TextureWrapMode wrapMode)
	{
		bind();
		GLCall(glTexParameteri
		(
			GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_T,
			RenderState::getTextureWrapModeOpenGLEnum(wrapMode)
		));
	}

	void Texture2D::setBorderColor(glm::vec4 color)
	{
		bind();
		GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &color.x));
	}

	void Texture2D::_create()
	{
		GLCall(glGenTextures(1, &m_id));
	}
	void Texture2D::_destroy()
	{
		GLCall(glDeleteTextures(1, &m_id));
	}

	void Texture2D::getFormat(const Image& image, unsigned& format, unsigned& internalFormat)
	{
		PK_ASSERT_QUICK(image.getNumChannels() >= 0);

		switch (image.getNumChannels())
		{
			case 1:    format = GL_RED;     internalFormat = GL_R8;       break;
			case 2:    format = GL_RG;      internalFormat = GL_RG8;      break;
			case 3:    format = GL_RGB;     internalFormat = GL_RGB8;     break;
			case 4:    format = GL_RGBA;    internalFormat = GL_RGBA8;    break;
			default: PK_LOG_ERROR("Trying to use an unsupported image format for a texture.", "Pekan"); break;
		}
	}

} // namespace Pekan
} // namespace Graphics