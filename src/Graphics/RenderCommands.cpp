#include "RenderCommands.h"
#include "PekanLogger.h"

#include "GLCall.h"

namespace Pekan
{
namespace Graphics
{

	void RenderCommands::draw(unsigned elementsCount, DrawMode mode)
	{
		GLCall(glDrawArrays(getDrawModeOpenGLEnum(mode), 0, elementsCount));
	}

	void RenderCommands::drawIndexed(unsigned elementsCount, DrawMode mode)
	{
		GLCall(glDrawElements(getDrawModeOpenGLEnum(mode), elementsCount, GL_UNSIGNED_INT, 0));
	}

	void RenderCommands::clear(bool doClearColorBuffer, bool doClearDepthBuffer)
	{
		if (doClearColorBuffer && doClearDepthBuffer)
		{
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))
		}
		else if (doClearColorBuffer)
		{
			GLCall(glClear(GL_COLOR_BUFFER_BIT));
		}
		else if (doClearDepthBuffer)
		{
			GLCall(glClear(GL_DEPTH_BUFFER_BIT));
		}
	}

	unsigned RenderCommands::getDrawModeOpenGLEnum(DrawMode drawMode)
	{
		switch (drawMode)
		{
			case DrawMode::Points:                    return GL_POINTS;
			case DrawMode::LineStrip:                 return GL_LINE_STRIP;
			case DrawMode::LineLoop:                  return GL_LINE_LOOP;
			case DrawMode::Lines:                     return GL_LINES;
			case DrawMode::LineStripAdjacency:        return GL_LINE_STRIP_ADJACENCY;
			case DrawMode::LinesAdjacency:            return GL_LINES_ADJACENCY;
			case DrawMode::TriangleStrip:             return GL_TRIANGLE_STRIP;
			case DrawMode::TriangleFan:               return GL_TRIANGLE_FAN;
			case DrawMode::Triangles:                 return GL_TRIANGLES;
			case DrawMode::TriangleStripAdjacency:    return GL_TRIANGLE_STRIP_ADJACENCY;
			case DrawMode::TrianglesAdjacency:        return GL_TRIANGLES_ADJACENCY;
			case DrawMode::Patches:                   return GL_PATCHES;
		}
		PK_ASSERT(false, "Unknown DrawMode, cannot determine OpenGL enum.", "Pekan");
		return 0;
	}

} // namespace Graphics
} // namespace Pekan