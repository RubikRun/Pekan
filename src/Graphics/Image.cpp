#include "Image.h"

#include "PekanLogger.h"
#include "Utils/FileUtils.h"

namespace Pekan
{
namespace Graphics
{

	bool Image::load(const char* filepath)
	{
		// Load image from file
		int width = -1, height = -1, numChannels = -1;
		const unsigned char* data = FileUtils::readImageFile(filepath, width, height, numChannels);
		// Check if loaded successfully
		if (width < 0 || height < 0 || numChannels < 0 || data == nullptr)
		{
			PK_LOG_ERROR("Failed to load image from file: " << filepath, "Pekan");
			return false;
		}

		// Only if successful, set members to loaded data
		m_width = width; m_height = height; m_numChannels = numChannels;
		m_data = data;

		return true;
	}

} // namespace Graphics
} // namespace Pekan