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

    int getRandomInt(glm::ivec2 range)
    {
        return getRandomInt(range.x, range.y);
    }

    float getRandomFloat(float min, float max)
    {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(getRandomNumberGenerator());
    }

    float getRandomFloat(glm::vec2 range)
    {
        return getRandomFloat(range.x, range.y);
    }

    glm::vec2 getRandomVec2(glm::vec2 xRange, glm::vec2 yRange)
    {
        return { getRandomFloat(xRange), getRandomFloat(yRange) };
    }

    glm::vec2 getRandomIVec2(glm::ivec2 xRange, glm::ivec2 yRange)
    {
        return { getRandomInt(xRange), getRandomInt(yRange) };
    }

} // namespace Utils
} // namespace Pekan
