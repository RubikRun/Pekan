#pragma once

#include "Texture2D.h"
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
		void create(int width, int height);
		void destroy();

		void bind() const;
		void unbind() const;

		// Binds the underlying texture so that its contents can be accessed
		// from outside the frame buffer.
		void bindTexture() const;

		// Checks if frame buffer is valid, meaning that it has been successfully created and not yet destroyed
		bool isValid() const;

	private: /* functions */

		// Creates underlying 2D texture object
		void createTexture(int width, int height);
		// Creates underlying render buffer
		void createRenderBuffer(int width, int height);

	private: /* variables */

		// Underlying 2D texture containing the colors of frame buffer's pixels.
		// Can be used in fragment shader to sample the frame buffer.
		Texture2D m_texture;

		// Underlying render buffer containing the depth and stencil values of frame buffer's pixels.
		RenderBuffer m_renderBuffer;

		// Frame buffer's ID on the GPU
		unsigned m_id = 0;
	};

} // namespace Graphics
} // namespace Pekan