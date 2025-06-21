#include "MathUtils.h"

namespace Pekan
{
namespace MathUtils
{

	bool isPointInTriangle(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c)
    {
        // Compute determinants of triangles PAB, PBC and PCA
        float detPAB = getDeterminant(p, a, b);
        float detPBC = getDeterminant(p, b, c);
        float detPCA = getDeterminant(p, c, a);

        // If one of those determinants is positive and another is negative,
        // then the point is NOT inside the triangle.
        return
        !(
            (detPAB < 0 || detPBC < 0 || detPCA < 0)
            && (detPAB > 0 || detPBC > 0 || detPCA > 0)
        );
    }

    bool isOrientationCCW(glm::vec2 a, glm::vec2 b, glm::vec2 c)
    {
        const float det = getDeterminant(a, b, c);
        return det > 0.0f;
    }

    float getDeterminant(glm::vec2 a, glm::vec2 b, glm::vec2 c)
    {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    }

} // namespace MathUtils
} // namespace Pekan
