#pragma once

#include <string>
#include <random>
#include <chrono>

namespace Pekan
{
namespace Utils
{

    // Reads a text file's contents into a string
    std::string readFileToString(const char* filePath);

    // Generates a random int in a given range
    int getRandomInt(int min, int max);

    // Generates a random float in a given range
    float getRandomFloat(float min, float max);

} // namespace Utils
} // namespace Pekan
