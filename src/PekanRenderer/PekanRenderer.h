#pragma once

#include <glm/glm.hpp>

#include "PekanEngine.h"

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

	// Enum for different types of blending factors
	enum class BlendFactor
	{
		Zero,
		One,
		SrcColor,
		OneMinusSrcColor,
		DstColor,
		OneMinusDstColor,
		SrcAlpha,
		OneMinusSrcAlpha,
		DstAlpha,
		OneMinusDstAlpha,
		ConstantColor,
		OneMinusConstantColor,
		ConstantAlpha,
		OneMinusConstantAlpha
	};

	// Enum for different types of usage of a buffer
	enum class BufferDataUsage
	{
		None = 0,
		StreamDraw = 1,
		StreamRead = 2,
		StreamCopy = 3,
		StaticDraw = 4,
		StaticRead = 5,
		StaticCopy = 6,
		DynamicDraw = 7,
		DynamicRead = 8,
		DynamicCopy = 9
	};

	// This is a singleton/static class containing common rendering functionality,
	// not specific to any graphics object, but just for the general rendering context.
	class PekanRenderer
	{
		friend class VertexArray;
		friend class VertexBufferElement;
		friend class VertexBuffer;
		friend class IndexBuffer;
		friend class Texture;

	public:

		// Draws elements from currently bound vertex buffer in the order that they appear
		static void draw(unsigned elementsCount, DrawMode mode = DrawMode::Triangles);

		// Draws elements from currently bound vertex buffer.
		// Uses currently bound index buffer to determine which elements to draw and in what order.
		static void drawIndexed(unsigned elementsCount, DrawMode mode = DrawMode::Triangles);

		// Clears everything rendered on window.
		// @param[in] clearDepthBuffer - Optional parameter to clear depth buffer as well
		static void clear(bool clearDepthBuffer = false);

		// Sets background's color, used to clear window
		static void setBackgroundColor(const glm::vec4& backgroundColor);

		// Enables blending capability
		static void enableBlending();

		// Sets the function used for blending.
		// This function is used to blend the incoming (source) RGBA values
		// with the RGBA values that are already in the frame buffer (destination).
		// NOTE: You need to enable blending with enableBlending() before using this function
		static void setBlendFunction(BlendFactor sourceFactor, BlendFactor destinationFactor);

		// Enables depth testing, so that overlapping primitives can be drawn correctly,
		// meaning that the ones in front will be rendered and the ones behind will be hidden.
		static void enableDepthTest();

	private: /* functions */

		// Returns the OpenGL base data type corresponding to the given shader data type.
		// Here "base type" means that the given shader data type can be multi-component
		// and the function will return the type of a single component of that type.
		static unsigned getShaderDataTypeOpenGLBaseType(ShaderDataType type);

		// Returns the OpenGL enum value corresponding to the given draw mode
		static unsigned getDrawModeOpenGLEnum(DrawMode drawMode);

		// Returns the OpenGL enum value corresponding to the given blend factor
		static unsigned getBlendFactorOpenGLEnum(BlendFactor blendFactor);

		// Returns the OpenGL enum value corresponding to the given buffer data usage
		static unsigned getBufferDataUsageOpenGLEnum(BufferDataUsage dataUsage);

		// Returns size in bytes of a given shader data type
		static unsigned getShaderDataTypeSize(ShaderDataType type);

		// Returns number of components of a given shader data type
		static unsigned getShaderDataTypeComponentsCount(ShaderDataType type);

		// Checks if a shader data type's base type is int
		static bool isShaderDataTypeInt(ShaderDataType type);

		// Returns the OpenGL enum value corresponding to the given texture slot
		static unsigned getTextureSlotOpenGLEnum(unsigned slot);
	};

} // namespace Renderer
} // namespace Pekan