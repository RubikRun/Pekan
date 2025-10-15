#include "RandomizationUtils.h"
#include "PekanLogger.h"

#include <random>
#include <chrono>

namespace Pekan
{
namespace RandomizationUtils
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

    glm::vec2 getRandomVec2(glm::vec2 min, glm::vec2 max)
    {
        return { getRandomFloat(min.x, max.x), getRandomFloat(min.y, max.y) };
    }

    glm::vec2 getRandomIVec2(glm::ivec2 min, glm::ivec2 max)
    {
        return { getRandomInt(min.x, max.x), getRandomInt(min.y, max.y) };
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

    glm::vec4 getRandomColor(float rgbMin, float rgbMax, bool randomizeAlpha)
    {
        return
        {
            getRandomFloat(rgbMin, rgbMax),
            getRandomFloat(rgbMin, rgbMax),
            getRandomFloat(rgbMin, rgbMax),
            randomizeAlpha ? getRandomFloat(0.0f, 1.0f) : 1.0f
        };
    }

    glm::vec4 getRandomColor(glm::vec3 rgbMin, glm::vec3 rgbMax, bool randomizeAlpha)
    {
        return
        {
            getRandomFloat(rgbMin.r, rgbMax.r),
            getRandomFloat(rgbMin.g, rgbMax.g),
            getRandomFloat(rgbMin.b, rgbMax.b),
            randomizeAlpha ? getRandomFloat(0.0f, 1.0f) : 1.0f
        };
    }

    glm::vec4 getRandomColor(glm::vec4 rgbaMin, glm::vec4 rgbaMax)
    {
        return
        {
            getRandomFloat(rgbaMin.r, rgbaMax.r),
            getRandomFloat(rgbaMin.g, rgbaMax.g),
            getRandomFloat(rgbaMin.b, rgbaMax.b),
            getRandomFloat(rgbaMin.a, rgbaMax.a),
        };
    }

} // namespace RandomizationUtils
} // namespace Pekan
