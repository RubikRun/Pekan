#include "FileUtils.h"

#include <stb/stb_image.h>

namespace Pekan
{
namespace FileUtils
{

	const unsigned char* readImageFile(const char* filepath, int& width, int& height, int& numChannels)
	{
		stbi_set_flip_vertically_on_load(true);
		return stbi_load(filepath, &width, &height, &numChannels, 0);
	}

} // namespace FileUtils
} // namespace Pekan
