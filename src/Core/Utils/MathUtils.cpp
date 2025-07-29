#include "MathUtils.h"

#include "PekanLogger.h"

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

    bool triangulatePolygon(const std::vector<glm::vec2>& vertices, std::vector<unsigned>& indices)
    {
        PK_ASSERT_QUICK(indices.empty());
        const size_t n = vertices.size();
        if (n < 3)
        {
            return true;
        }

        std::vector<unsigned> remaining;
        for (unsigned i = 0; i < n; ++i)
        {
            remaining.push_back(i);
        }

        // A lambda function checking if a given index in the "remaining" list is an ear.
        // An "ear" means a vertex from the remaining vertices,
        // that together with the previous and next vertex from the remaining vertices
        // form a CCW angle, AND the triangle that they form does NOT contain any of the other remaining vertices.
        const auto isEar = [&](int i) -> bool
        {
            const unsigned prev = remaining[(i - 1 + remaining.size()) % remaining.size()];
            const unsigned curr = remaining[i];
            const unsigned next = remaining[(i + 1) % remaining.size()];

            if (!MathUtils::isOrientationCCW(vertices[prev], vertices[curr], vertices[next]))
            {
                return false;
            }

            const glm::vec2 a = vertices[prev];
            const glm::vec2 b = vertices[curr];
            const glm::vec2 c = vertices[next];

            for (unsigned j : remaining)
            {
                if (j == prev || j == curr || j == next)
                {
                    continue;
                }
                if (MathUtils::isPointInTriangle(vertices[j], a, b, c))
                {
                    return false;
                }
            }

            return true;
        };

        // Repeat until we have exactly 3 remaining vertices
        while (remaining.size() > 3)
        {
            bool earFound = false;
            // Loop over remaining vertices and look for an ear
            for (size_t i = 0; i < remaining.size(); ++i)
            {
                // If we find an ear
                if (isEar(i))
                {
                    // then use the ear vertex, together with the previous and next vertex.
                    const unsigned prev = remaining[(i - 1 + remaining.size()) % remaining.size()];
                    const unsigned curr = remaining[i];
                    const unsigned next = remaining[(i + 1) % remaining.size()];

                    // These 3 vertices are pushed into the "indices" array because they form a finalized triangle
                    // that needs to be rendered.
                    indices.push_back(prev);
                    indices.push_back(curr);
                    indices.push_back(next);
                    // Finally, the ear vertex itself is removed from the "remaining" vertices,
                    // effectively cutting out the triangle from the polygon.
                    remaining.erase(remaining.begin() + i);
                    earFound = true;
                    break;
                }
            }

            if (!earFound)
            {
                return false;
            }
        }

        // It is expected that at the end there will be exactly 3 vertices left.
        // 3 vertices for a triangle, so no need to triangulate more.
        // We are done. Push those final 3 vertices to the "indices" list, in CCW order.
        if (remaining.size() == 3)
        {
            int a = remaining[0];
            int b = remaining[1];
            int c = remaining[2];
            if (!MathUtils::isOrientationCCW(vertices[a], vertices[b], vertices[c]))
            {
                std::swap(a, b);
            }
            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(c);
        }

        return true;
    }

    bool isPolygonConvex(const std::vector<glm::vec2>& vertices)
    {
        if (vertices.size() < 3)
        {
            return false;
        }

        bool foundCW = false;
        bool foundCCW = false;

        // Traverse polygon's vertices
        const int n = int(vertices.size());
        for (int i = 0; i < n; ++i)
        {
            // Reference the current 3 consecutive vertices A, B, C
            const glm::vec2& a = vertices[i];
            const glm::vec2& b = vertices[(i + 1) % n];
            const glm::vec2& c = vertices[(i + 2) % n];
            // Compute determinant of ABC
            const float detABC = MathUtils::getDeterminant(a, b, c);

            if (detABC > 0)
            {
                foundCW = true;
            }
            else if (detABC < 0)
            {
                foundCCW = true;
            }

            // If there are both positive and negative determinants,
            // this means that the polygon's angles sometimes turn left and sometimes right,
            // which means that it's concave, hence not convex.
            if (foundCW && foundCCW)
            {
                return false;
            }
        }

        // At this point all determinants have been either positive or negative,
        // so all angles are turning either to the left or to the right,
        // which means that the polygon is convex
        return true;
    }

    bool isPolygonCCW(const std::vector<glm::vec2>& vertices)
    {
        const size_t n = vertices.size();
        // Not a valid polygon, if vertices are less than 3
        if (n < 3)
        {
            return false;
        }

        // Compute sum of signed area,
        // using an optimized version of the "Shoelace Formula".
        float areaSum = 0.0f;
        for (size_t i = 0; i < n; ++i)
        {
            const glm::vec2& a = vertices[i];
            const glm::vec2& b = vertices[(i + 1) % n];
            areaSum += (b.x - a.x) * (b.y + a.y);
        }

        // If the signed area sum is negative, this means that the polygon is CCW
        return areaSum < 0.0f;
    }

    void updateTriangleFanIndices(std::vector<unsigned>& indices, int nVertices)
    {
        PK_ASSERT
        (
            indices.size() % 3 == 0,
            "Trying to update a list of triangle fan indices but given list contains an invalid number of indices.",
            "Pekan"
        );
        PK_DEBUG_CODE
        (
            for (size_t i = 0; i + 2 < indices.size(); i += 3)
            {
                PK_ASSERT
                (
                    indices[i] == 0 && indices[i + 1] == i / 3 + 1 && indices[i + 2] == i / 3 + 2,
                    "Trying to update a list of triangle fan indices but indices in given list are not in correct triangle fan format.",
                    "Pekan"
                );
            }
        );

        const size_t prevIndicesCount = indices.size();
        indices.resize((nVertices - 2) * 3);
        for (int i = prevIndicesCount; i + 2 < indices.size(); i += 3)
        {
            indices[i] = 0;
            indices[i + 1] = i / 3 + 1;
            indices[i + 2] = i / 3 + 2;
        }
    }

    void generateTriangleFanIndices(unsigned* indices, int nVertices)
    {
        for (int i = 0; i + 2 < (nVertices - 2) * 3; i += 3)
        {
            indices[i] = 0;
            indices[i + 1] = i / 3 + 1;
            indices[i + 2] = i / 3 + 2;
        }
    }

    bool isOrientationReversedByTransform(const glm::mat3& transform)
    {
        const glm::vec2 a = glm::vec2(transform[0]);
        const glm::vec2 b = glm::vec2(transform[1]);
        const float det = a.x * b.y - a.y * b.x;
        return det < 0.0f;
    }

} // namespace MathUtils
} // namespace Pekan
