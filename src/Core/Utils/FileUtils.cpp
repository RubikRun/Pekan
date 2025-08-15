#include "FileUtils.h"
#include "PekanLogger.h"

#include <stb/stb_image.h>

#include <fstream>
#include <sstream>

namespace Pekan
{
namespace FileUtils
{

    std::string readTextFileToString(const char* filepath)
    {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            PK_LOG_ERROR("Failed to open file for reading: " << filepath, "Pekan");
            return std::string();
        }

        std::ostringstream buffer;
        buffer << file.rdbuf();

        file.close();
        if (file.is_open())
        {
            PK_LOG_ERROR("Failed to close file: " << filepath, "Pekan");
        }

        return buffer.str();
    }

    void writeStringToTextFile(const char* filepath, const char* content)
    {
        std::ofstream file(filepath);
        if (!file.is_open())
        {
            PK_LOG_ERROR("Failed to write file: " << filepath, "Pekan");
            return;
        }

        file << content;

        file.close();
        if (file.is_open())
        {
            PK_LOG_ERROR("Failed to close file: " << filepath, "Pekan");
        }
    }

	const unsigned char* readImageFile(const char* filepath, int& width, int& height, int& numChannels)
	{
		stbi_set_flip_vertically_on_load(true);
		return stbi_load(filepath, &width, &height, &numChannels, 0);
	}

} // namespace FileUtils
} // namespace Pekan
