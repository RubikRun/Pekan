#include "Utils/PekanUtils.h"
#include "Logger/PekanLogger.h"

#include <fstream>
#include <sstream>

namespace Pekan
{
namespace Utils
{

    // Returns the random number generator.
    // If called for the first time it will also seed the random number generator.
    static inline std::mt19937& getRandomNumberGenerator()
    {
        static std::mt19937 randomNumberGenerator(std::chrono::steady_clock::now().time_since_epoch().count());
        return randomNumberGenerator;
    }

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

    int getRandomInt(int min, int max)
    {
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(getRandomNumberGenerator());
    }

    float getRandomFloat(float min, float max)
    {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(getRandomNumberGenerator());
    }

} // namespace Utils
} // namespace Pekan
