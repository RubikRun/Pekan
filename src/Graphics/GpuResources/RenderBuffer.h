#pragma once

namespace Pekan
{
namespace Graphics
{

	class FrameBuffer;

	// A class representing a render buffer on the GPU.
	//
	// NOTE: So far we support only render buffers with 24 depth bits and 8 stencil bits per pixel
	class RenderBuffer
	{
	public:

		~RenderBuffer();

		// Creates the render buffer object, and binds it
		void create(int width, int height, int samplesPerPixel = 1);
		void destroy();

		void bind() const;
		void unbind() const;

		// Attaches render buffer to a given frame buffer
		void attachToFrameBuffer(const FrameBuffer& frameBuffer) const;

		// Checks if render buffer is valid, meaning that it has been successfully created and not yet destroyed
		bool isValid() const { return m_id != 0; }

	private: /* variables */

		// Frame buffer's ID on the GPU
		unsigned m_id = 0;
	};

} // namespace Graphics
} // namespace Pekan