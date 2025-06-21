#pragma once

#include <glm/glm.hpp>

namespace Pekan
{
namespace MathUtils
{
    // Checks if a given 2D point P is inside of a given triangle ABC
    bool isPointInTriangle(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c);

    // Checks if the orientation of 3 given points ABC is counter-clockwise,
    // meaning that if you start at A, then go to B and then to C, you will turn left at point B.
    bool isOrientationCCW(glm::vec2 a, glm::vec2 b, glm::vec2 c);

    // Returns determinant of 3 vertices A, B, C.
    // More precisely returns the determinant of vector AB and vector AC.
    float getDeterminant(glm::vec2 a, glm::vec2 b, glm::vec2 c);

} // namespace MathUtils
} // namespace Pekan
