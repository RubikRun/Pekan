#pragma once

namespace Pekan
{
namespace Graphics
{

	class Image
	{
	public:

		Image() = default;
		Image(const char* filepath) { load(filepath); }

		// Loads image from given image file
		bool load(const char* filepath);

		inline const unsigned char* getData() const { return m_data; }

		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }
		inline int getNumChannels() const { return m_numChannels; }

		// Checks if image is valid, meaning it has been loaded successfully and contains valid data
		bool isValid() const { return m_data != nullptr; }

	private:

		// Pixel data containing the actual image
		const unsigned char* m_data = nullptr;

		// Width of image, in pixels
		int m_width = -1;
		// Height of image, in pixels
		int m_height = -1;

		// Number of color channels, usually 1, 3 or 4
		int m_numChannels = -1;
	};

} // namespace Graphics
} // namespace Pekan