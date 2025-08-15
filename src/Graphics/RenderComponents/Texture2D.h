#pragma once

#include "RenderState.h"

#include <memory>

namespace Pekan {
namespace Graphics {

	class Image;

	// A class representing a 2D texture on the GPU.
	class Texture2D
	{
	public:

		~Texture2D();

		// Creates an empty texture
		void create();
		// Creates a texture from a given image
		void create(const Image& image);
		void destroy();

		// Sets a new image to the texture
		void setImage(const Image& image);

		// Binds/unbinds texture to currently active texture slot
		void bind() const;
		void unbind() const;
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

		// Checks if texture is valid, meaning that it has been successfully created and not yet destroyed
		bool isValid() const { return m_id != 0; }

	private: /* functions */

		// Determines the format (and internal format) that a texture must have to support a given image
		static void getFormat(const Image& image, unsigned& format, unsigned& internalFormat);

	private: /* variables */

		// Texture's ID on the GPU
		unsigned m_id = 0;
	};

	typedef std::shared_ptr<Texture2D> Texture2D_Ptr;
	typedef std::shared_ptr<const Texture2D> Texture2D_ConstPtr;

} // namespace Pekan
} // namespace Graphics