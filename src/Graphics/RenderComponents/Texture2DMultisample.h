#pragma once

#include "RenderState.h"

#include <memory>

namespace Pekan {
namespace Graphics {

	class Image;
	class FrameBuffer;

	// A class representing a multisample 2D texture on the GPU.
	class Texture2DMultisample
	{
	public:

		~Texture2DMultisample();

		// Creates an empty texture
		void create(int samplesPerTexel);
		void destroy();

		// Sets texture's size,
		// allocating memory for that many texels,
		// but NOT filling them with data.
		void setSize(int width, int height, int numChannels = 4);

		// Binds/unbinds texture to currently active texture slot
		void bind() const;
		void unbind() const;
		// Binds/unbinds texture to given texture slot
		void bind(unsigned slot) const;
		void unbind(unsigned slot) const;

		// Activates given texture slot
		static void activateSlot(unsigned slot);

		// Attaches texture to a given frame buffer
		void attachToFrameBuffer(const FrameBuffer& frameBuffer) const;

		// Checks if texture is valid, meaning that it has been successfully created and not yet destroyed
		bool isValid() const { return m_id != 0; }

	private: /* functions */

		// Determines the format (and internal format) that a texture must have to support a given number of channels
		static void getFormat(int numChannels, unsigned& format, unsigned& internalFormat);

	private: /* variables */

		// Number of samples per texel
		int m_samplesPerTexel = -1;

		// Texture's ID on the GPU
		unsigned m_id = 0;
	};

	typedef std::shared_ptr<Texture2DMultisample> Texture2DMultisample_Ptr;
	typedef std::shared_ptr<const Texture2DMultisample> Texture2DMultisample_ConstPtr;

} // namespace Pekan
} // namespace Graphics