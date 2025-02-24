#pragma once

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer
{

	// Enum for shader data types that we support in Pekan.
	// They are mapped to concrete data types of GLSL, HLSL, etc.
	enum class ShaderDataType
	{
		None = 0, Float = 1, Float2 = 2, Float3 = 3, Float4 = 4, Mat3 = 5, Mat4 = 6, Int = 7, Int2 = 8, Int3 = 9, Int4 = 10, Bool = 11
	};

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

	class PekanRenderer
	{
	public:

		// Draws elements from currently bound vertex buffer in the order that they appear
		static void draw(unsigned elementsCount, DrawMode mode = DrawMode::Triangles);

		// Draws elements from currently bound vertex buffer.
		// Uses currently bound index buffer to determine which elements to draw and in what order.
		static void drawIndexed(unsigned elementsCount, DrawMode mode = DrawMode::Triangles);

		// Clears everything rendered on window
		static void clear();

		// Sets background's color, used to clear window
		static void setBackgroundColor(const glm::vec4& backgroundColor);

		// Returns the OpenGL enum value corresponding to the given draw mode
		static unsigned getDrawModeOpenGLEnum(DrawMode drawMode);

		// Returns the OpenGL base data type corresponding to the given shader data type.
		// Here "base type" means that the given shader data type can be multi-component
		// and the function will return the type of a single component of that type.
		static unsigned getShaderDataTypeOpenGLBaseType(ShaderDataType type);

		// Returns size in bytes of a given shader data type
		static unsigned getShaderDataTypeSize(ShaderDataType type);

		// Returns number of components of a given shader data type
		static unsigned getShaderDataTypeComponentsCount(ShaderDataType type);
	};

} // namespace Renderer
} // namespace Pekan