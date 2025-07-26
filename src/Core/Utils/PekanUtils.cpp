#include "Utils/PekanUtils.h"
#include "PekanLogger.h"

#include <random>
#include <chrono>

namespace Pekan
{
namespace Utils
{

    // Returns the random number generator.
    // If called for the first time it will also seed the random number generator.
    static std::mt19937& getRandomNumberGenerator()
    {
        static std::mt19937 randomNumberGenerator(std::chrono::steady_clock::now().time_since_epoch().count());
        return randomNumberGenerator;
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

    glm::vec4 getRandomColor(bool randomizeAlpha)
    {
        return
        {
            getRandomFloat(0.0f, 1.0f),
            getRandomFloat(0.0f, 1.0f),
            getRandomFloat(0.0f, 1.0f),
            randomizeAlpha ? getRandomFloat(0.0f, 1.0f) : 1.0f
        };
    }

    glm::vec4 getRandomColor(float rgbLowerBound, float rgbUpperBound, bool randomizeAlpha)
    {
        return
        {
            getRandomFloat(rgbLowerBound, rgbUpperBound),
            getRandomFloat(rgbLowerBound, rgbUpperBound),
            getRandomFloat(rgbLowerBound, rgbUpperBound),
            randomizeAlpha ? getRandomFloat(0.0f, 1.0f) : 1.0f
        };
    }

} // namespace Utils
} // namespace Pekan
