#include "FileUtils.h"
#include "Logger/PekanLogger.h"

#include <stb/stb_image.h>

#include <fstream>
#include <sstream>

namespace Pekan
{
namespace FileUtils
{

    std::string readFileToString(const char* filePath)
    {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            PK_LOG_ERROR("Failed to open file: " << filePath, "Pekan");
            return std::string();
        }

        std::ostringstream buffer;
        buffer << file.rdbuf();

        file.close();
        if (file.is_open())
        {
            PK_LOG_ERROR("Failed to close file: " << filePath, "Pekan");
        }

        return buffer.str();
    }

	const unsigned char* readImageFile(const char* filepath, int& width, int& height, int& numChannels)
	{
		stbi_set_flip_vertically_on_load(true);
		return stbi_load(filepath, &width, &height, &numChannels, 0);
	}

} // namespace FileUtils
} // namespace Pekan
