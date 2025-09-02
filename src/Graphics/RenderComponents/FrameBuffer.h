#pragma once

#include "Texture2D.h"
#include "Texture2DMultisample.h"
#include "RenderBuffer.h"

namespace Pekan
{
namespace Graphics
{

	// A class representing a frame buffer on the GPU.
	class FrameBuffer
	{
	public:

		~FrameBuffer();

		// Creates the frame buffer object, and binds it
		void create(int width, int height, int samplesPerPixel = 1);
		void destroy();

		void bind() const;
		void unbind() const;

		// Binds the underlying texture so that its contents can be accessed
		// from outside the frame buffer.
		void bindTexture() const;

		// Resolves a multisample frame buffer to a given target single-sample buffer,
		// effectively copying all pixel data, but changing it from multisample to single-sample.
		void resolveMultisampleToSinglesample(FrameBuffer& targetFrameBuffer);

		// Checks if frame buffer is valid, meaning that it has been successfully created and not yet destroyed
		bool isValid() const;

	private: /* functions */

		// Creates underlying 2D texture object
		void createTexture();
		void createTextureMultisample();
		// Creates underlying render buffer
		void createRenderBuffer();

	private: /* variables */

		// Number of samples per pixel
		int m_samplesPerPixel = -1;

		// Width and height of frame buffer, in pixels
		int m_width = -1;
		int m_height = -1;

		// Underlying 2D texture containing the colors of frame buffer's pixels.
		// Can be used in fragment shader to sample the frame buffer.
		//
		// NOTE: Only one of these 2 textures will be used depending on the number of samples per pixel.
		Texture2D m_texture;
		Texture2DMultisample m_textureMultisample;

		// Underlying render buffer containing the depth and stencil values of frame buffer's pixels.
		RenderBuffer m_renderBuffer;

		// Frame buffer's ID on the GPU
		unsigned m_id = 0;
	};

} // namespace Graphics
} // namespace Pekan