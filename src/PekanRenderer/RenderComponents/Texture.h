#pragma once

#include "RenderComponent.h"

namespace Pekan {
namespace Renderer {

	class Image;



	// A class representing a texture in Pekan's renderer
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

	private: /* functions */

		void _create() override;
		void _destroy() override;

		// Determines the format (and internal format) that a texture must have to support a given image
		static void getFormat(const Image& image, unsigned& format, unsigned& internalFormat);
	};

	typedef std::shared_ptr<Texture> TexturePtr;

} // namespace Pekan
} // namespace Renderer