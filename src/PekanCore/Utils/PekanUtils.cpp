#include "Utils/PekanUtils.h"
#include "Logger/PekanLogger.h"

#include <fstream>
#include <sstream>

namespace Pekan
{
namespace Utils
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

} // namespace Utils
} // namespace Pekan
