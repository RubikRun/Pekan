#pragma once

#include <glm/glm.hpp>

namespace Pekan
{
namespace Utils
{

    // Generates a random int in a given range
    int getRandomInt(int min, int max);
    int getRandomInt(glm::ivec2 range);

    // Generates a random float in a given range
    float getRandomFloat(float min, float max);
    float getRandomFloat(glm::vec2 range);

    // Generates a random vec2 (2 float values) in a given range
    glm::vec2 getRandomVec2(glm::vec2 xRange, glm::vec2 yRange);

    // Generates a random ivec2 (2 int values) in a given range
    glm::vec2 getRandomIVec2(glm::ivec2 xRange, glm::ivec2 yRange);

    // Generates a random color in RGBA format using a [0, 1] range
    // @param[in] randomizeAlpha - specifies if alpha value should be random as well.
    //                             If this parameter is false, you'll get a random solid color.
    //                             If this parameter is true, you'll get a random color with a random opacity.
    // @param[in] rgbLowerBound - Minimum allowed value for the R, G and B components
    // @param[in] rgbUpperBound - Maximum allowed value for the R, G and B components
    glm::vec4 getRandomColor(bool randomizeAlpha = false);
    glm::vec4 getRandomColor(float rgbLowerBound, float rgbUpperBound, bool randomizeAlpha = false);

} // namespace Utils
} // namespace Pekan
