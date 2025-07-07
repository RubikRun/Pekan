#pragma once

#include "PekanEngine.h"

namespace Pekan
{
namespace Graphics
{

	// Enum for different types of draw modes. Each mode specifies a kind of primitive to render.
	enum class DrawMode
	{
		Points = 0,
		LineStrip = 1,
		LineLoop = 2,
		Lines = 3,
		LineStripAdjacency = 4,
		LinesAdjacency = 5,
		TriangleStrip = 6,
		TriangleFan = 7,
		Triangles = 8,
		TriangleStripAdjacency = 9,
		TrianglesAdjacency = 10,
		Patches = 11
	};

	// A singleton/static class containing low-level render commands.
	class RenderCommands
	{
	public:

		// Draws elements from currently bound vertex buffer in the order that they appear
		static void draw(unsigned elementsCount, DrawMode mode = DrawMode::Triangles);

		// Draws elements from currently bound vertex buffer.
		// Uses currently bound index buffer to determine which elements to draw and in what order.
		static void drawIndexed(unsigned elementsCount, DrawMode mode = DrawMode::Triangles);

		// Clears everything rendered on window.
		// @param[in] clearDepthBuffer - Optional parameter to clear depth buffer as well
		static void clear(bool clearDepthBuffer = false);

	private:

		// Returns the OpenGL enum value corresponding to the given draw mode
		static unsigned getDrawModeOpenGLEnum(DrawMode drawMode);
	};

} // namespace Graphics
} // namespace Pekan