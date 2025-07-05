#pragma once

#include "RenderState.h"
#include "RenderComponent.h"

#include <memory>

namespace Pekan {
namespace Graphics {

	class Image;



	// A class representing a texture on the GPU.
	class Texture : public RenderComponent
	{
	public:

		~Texture();

		// Make base class RenderComponent's version of create() be visible in this derived class
		using RenderComponent::create;

		void create(const Image& image);

		void setImage(const Image& image);

		// Binds/unbinds texture to currently active texture slot
		void bind() const override;
		void unbind() const override;
		// Binds/unbinds texture to given texture slot
		void bind(unsigned slot) const;
		void unbind(unsigned slot) const;

		// Activates given texture slot
		static void activateSlot(unsigned slot);

		// Sets a minify function to be used for sampling the texture
		void setMinifyFunction(TextureMinifyFunction function);
		// Sets a magnify function to be used for sampling the texture
		void setMagnifyFunction(TextureMagnifyFunction function);

		// Sets a wrap mode to be used for the X texture coordinate
		void setWrapModeX(TextureWrapMode wrapMode);
		// Sets a wrap mode to be used for the Y texture coordinate
		void setWrapModeY(TextureWrapMode wrapMode);

		// Sets a color for the border of the texture.
		// This color will be used to color pixels outside of the [0, 1] range if the wrap mode is ClampToBorder.
		void setBorderColor(glm::vec4 color);

	private: /* functions */

		void _create() override;
		void _destroy() override;

		// Determines the format (and internal format) that a texture must have to support a given image
		static void getFormat(const Image& image, unsigned& format, unsigned& internalFormat);
	};

	typedef std::shared_ptr<Texture> TexturePtr;

} // namespace Pekan
} // namespace Graphics