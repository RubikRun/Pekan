#include "Utils/PekanUtils.h"
#define PK_FILENAME "PekanUtils.cpp"
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
            PK_LOG_ERRORF("Failed to open file: " << filePath);
            return std::string();
        }

        std::ostringstream buffer;
        buffer << file.rdbuf();

        file.close();
        if (file.is_open())
        {
            PK_LOG_ERRORF("Failed to close file: " << filePath);
        }

        return buffer.str();
    }

} // namespace Utils
} // namespace Pekan
