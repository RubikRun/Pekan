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

    // Triangulates a polygon formed by the given vertices,
    // using the "Ear Clipping" algorithm.
    //
    // NOTE: Expects that the given vertices will be in CCW order
    //
    // @param[out] indices - Fills list with indices into the vertices
    // such that every 3 consecutive indices form a triangle,
    // and all those triangles combine to the exact same shape as the polygon.
    // @return true on success
    bool triangulatePolygon(const std::vector<glm::vec2>& vertices, std::vector<unsigned>& indices);

    // Checks if given vertices form a convex polygon
    bool isPolygonConvex(const std::vector<glm::vec2>& vertices);

    // Checks if given vertices form a CCW (counter-clockwise) polygon
    bool isPolygonCCW(const std::vector<glm::vec2>& vertices);

    // Updates a list of "triangle fan" indices for a given number of vertices.
    // A list of "triangle fan" indices is of the form { 0, 1, 2, 0, 2, 3, 0, 3, 4, ... }.
    // For example if number of vertices is 6, then indices will be { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5 }.
    //
    // NOTE: If the given "indices" list already contains some indices, they will be reused.
    //       For example if it already contains { 0, 1, 2, 0, 2, 3 } and now we are requesting number of vertices 5,
    //       then the function will only extend the indices list with { 0, 3, 4 } resulting in { 0, 1, 2, 0, 2, 3, 0, 3, 4 }.
    //       If the given "indices" list contains more indices than needed, it will be shortened.
    void updateTriangleFanIndices(std::vector<unsigned>& indices, int nVertices);

    // Generates "triangle fan" indices in the form { 0, 1, 2, 0, 2, 3, 0, 3, 4, ... } for a given number of vertices.
    // For example if number of vertices is 6, the indices will be { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5 }
    // Resulting indices will be filled into the given "indices" array.
    //
    // WARNING: Indices array must be of size (nVertices - 2) * 3 to contain the nVertices - 2 triangles.
    void generateTriangleFanIndices(unsigned* indices, int nVertices);

    // Checks if orientation is reversed by a given transform matrix
    bool isOrientationReversedByTransform(const glm::mat3& transform);

} // namespace MathUtils
} // namespace Pekan
