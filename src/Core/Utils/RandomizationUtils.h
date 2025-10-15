#pragma once

#include <glm/glm.hpp>

namespace Pekan
{
namespace RandomizationUtils
{

    // Generates a random float in a given range
    float getRandomFloat(float min, float max);
    // Generates a random int in a given range
    int getRandomInt(int min, int max);

    // Generates a random vec2 in a given range
    glm::vec2 getRandomVec2(glm::vec2 min, glm::vec2 max);
    // Generates a random ivec2 in a given range
    glm::vec2 getRandomIVec2(glm::ivec2 min, glm::ivec2 max);

    // Generates a random RGBA color in [0,1] range.
    // @param[in] randomizeAlpha - Specifies if alpha value should be random as well. If not, it will be 1.0.
    glm::vec4 getRandomColor(bool randomizeAlpha = false);
    // Generates a random RGBA color in given RGB range.
    // @param[in] rgbMin - Minimum allowed values for the R, G and B components
    // @param[in] rgbMax - Maximum allowed values for the R, G and B components
    // @param[in] randomizeAlpha - Specifies if alpha value should be random as well. If not, it will be 1.0.
    glm::vec4 getRandomColor(glm::vec3 rgbMin, glm::vec3 rgbMax, bool randomizeAlpha = false);
    // Generates a random RGBA color in given RGBA range.
    // @param[in] rgbaMin - Minimum allowed values for the R, G, B and A components
    // @param[in] rgbaMax - Maximum allowed values for the R, G, B and A components
    glm::vec4 getRandomColor(glm::vec4 rgbaMin, glm::vec4 rgbaMax);

} // namespace RandomizationUtils
} // namespace Pekan
