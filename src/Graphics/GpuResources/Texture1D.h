#pragma once

#include "RenderState.h"

#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace Pekan {
namespace Graphics {

	// A class representing a 1D texture on the GPU.
	class Texture1D
	{
	public:

		~Texture1D();

		// Creates an empty 1D texture
		void create();
		// Creates a 1D texture from a list of RGBA colors
		void create(const std::vector<glm::vec4>& colors);
		void destroy();

		// Sets new colors to the texture
		void setColors(const std::vector<glm::vec4>& colors);

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

		// Sets a wrap mode to be used for texture coorindates
		void setWrapMode(TextureWrapMode wrapMode);

		// Sets a color for the border of the texture.
		// This color will be used to color pixels outside of the [0, 1] range if the wrap mode is ClampToBorder.
		void setBorderColor(glm::vec4 color);

		// Checks if texture is valid, meaning that it has been successfully created and not yet destroyed
		bool isValid() const { return m_id != 0; }

	private: /* variables */

		// Texture's ID on the GPU
		unsigned m_id = 0;
	};

	typedef std::shared_ptr<Texture1D> Texture1DPtr;

} // namespace Graphics
} // namespace Pekan
