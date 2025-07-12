#include "Texture1D.h"

#include "PekanLogger.h"
#include "GLCall.h"

#include <glm/glm.hpp>

namespace Pekan {
namespace Graphics {

	static const TextureMinifyFunction DEFAULT_TEXTURE_MINIFY_FUNCTION = TextureMinifyFunction::Nearest;
	static const TextureMagnifyFunction DEFAULT_TEXTURE_MAGNIFY_FUNCTION = TextureMagnifyFunction::Nearest;
	static const TextureWrapMode DEFAULT_WRAP_MODE = TextureWrapMode::ClampToBorder;
	static const glm::vec4 DEFAULT_BORDER_COLOR = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	Texture1D::~Texture1D()
	{
		if (isValid())
		{
			destroy();
		}
	}

	void Texture1D::create()
	{
		RenderComponent::create();

		// Configure the default minify and magnify functions of the texture
		setMinifyFunction(DEFAULT_TEXTURE_MINIFY_FUNCTION);
		setMagnifyFunction(DEFAULT_TEXTURE_MAGNIFY_FUNCTION);

		// Configure texture wrapping
		setWrapMode(DEFAULT_WRAP_MODE);
		setBorderColor(DEFAULT_BORDER_COLOR);
	}

	void Texture1D::create(const std::vector<glm::vec4>& colors)
	{
		create();
		setColors(colors);
	}

	void Texture1D::setColors(const std::vector<glm::vec4>& colors)
	{
		bind();

		// Set colors data to the texture object
		const unsigned textureSize = static_cast<unsigned>(colors.size());
		GLCall(glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, textureSize, 0, GL_RGBA, GL_FLOAT, colors.data()));
	}

	void Texture1D::bind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_1D, m_id));
	}

	void Texture1D::unbind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_1D, 0));
	}

	void Texture1D::bind(unsigned slot) const
	{
		activateSlot(slot);
		GLCall(glBindTexture(GL_TEXTURE_1D, m_id));
	}

	void Texture1D::unbind(unsigned slot) const
	{
		activateSlot(slot);
		GLCall(glBindTexture(GL_TEXTURE_1D, 0));
	}

	void Texture1D::activateSlot(unsigned slot)
	{
		const unsigned slotEnumValue = RenderState::getTextureSlotOpenGLEnum(slot);
		GLCall(glActiveTexture(slotEnumValue));
	}

	void Texture1D::setMinifyFunction(TextureMinifyFunction function)
	{
		bind();
		GLCall(glTexParameteri
		(
			GL_TEXTURE_1D,
			GL_TEXTURE_MIN_FILTER,
			RenderState::getTextureMinifyFunctionOpenGLEnum(function)
		));
	}

	void Texture1D::setMagnifyFunction(TextureMagnifyFunction function)
	{
		bind();
		GLCall(glTexParameteri
		(
			GL_TEXTURE_1D,
			GL_TEXTURE_MAG_FILTER,
			RenderState::getTextureMagnifyFunctionOpenGLEnum(function)
		));
	}

	void Texture1D::setWrapMode(TextureWrapMode wrapMode)
	{
		bind();
		GLCall(glTexParameteri
		(
			GL_TEXTURE_1D,
			GL_TEXTURE_WRAP_S,
			RenderState::getTextureWrapModeOpenGLEnum(wrapMode)
		));
	}

	void Texture1D::setBorderColor(glm::vec4 color)
	{
		bind();
		GLCall(glTexParameterfv(GL_TEXTURE_1D, GL_TEXTURE_BORDER_COLOR, &color.x));
	}

	void Texture1D::_create()
	{
		GLCall(glGenTextures(1, &m_id));
	}

	void Texture1D::_destroy()
	{
		GLCall(glDeleteTextures(1, &m_id));
	}

} // namespace Graphics
} // namespace Pekan
