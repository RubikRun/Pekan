#pragma once

#include "PekanEngine.h"

namespace Pekan
{
namespace Graphics
{

	// Enum for shader data types that we support in Pekan.
	// They are mapped to concrete data types of GLSL, HLSL, etc.
	enum class ShaderDataType
	{
		None = 0, Float = 1, Float2 = 2, Float3 = 3, Float4 = 4, Mat3 = 5, Mat4 = 6, Int = 7, Int2 = 8, Int3 = 9, Int4 = 10, Bool = 11
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

	// Enum for different minify functions that can be used for texture sampling.
	// A minify function is used when a texture has higher frequency than our sampling.
	// So for each of our samples we might consider multiple texels.
	enum class TextureMinifyFunction
	{
		// Uses the value of the texture element that is nearest (in Manhattan distance) to the specified texture coordinates
		Nearest = 0,
		// Uses the weighted average of the four texture elements that are closest to the specified texture coordinates
		Linear = 1,
		// Chooses the nearest mipmap - the one that most closely matches the size of the pixel being textured,
		// and uses the Nearest criterion to produce a texture value
		NearestOnNearestMipmap = 2,
		// Chooses the nearest mipmap - the one that most closely matches the size of the pixel being textured,
		// and uses the Linear criterion to produce a texture value
		LinearOnNearestMipmap = 3,
		// Chooses the two mipmaps that most closely match the size of the pixel being textured
		// and uses the Nearest criterion to produce a texture value from each mipmap.
		// The final texture value is a weighted average of those two values
		NearestOnLinearMipmap = 4,
		// Chooses the two mipmaps that most closely match the size of the pixel being textured
		// and uses the Linear criterion to produce a texture value from each mipmap.
		// The final texture value is a weighted average of those two values
		LinearOnLinearMipmap = 5
	};

	// Enum for different minify functions that can be used for texture sampling.
	// A magnify function is used when a texture has lower frequency than our sampling.
	// So each texel might be used for multiple of our samples.
	enum class TextureMagnifyFunction
	{
		// Uses the value of the texture element that is nearest (in Manhattan distance) to the specified texture coordinates
		Nearest = 0,
		// Uses the weighted average of the four texture elements that are closest to the specified texture coordinates
		Linear = 1,
	};

	// Enum for different modes of wrapping that can be used with textures.
	// Wrapping is used when a given texture coordinate t, could be either an X coordinate or a Y coordinate,
	// is outside of the [0, 1] range.
	enum class TextureWrapMode
	{
		// Clamps t to the [0, 1] range, hence to the edge of the texture.
		ClampToEdge = 0,
		// Repeats the texture.
		// The integer part of t is ignored, only the fractional part frac(t) is used,
		// causing a repeating pattern.
		Repeat = 1,
		// Repeats the texture, but mirrors it every second time.
		// If the integer part of t is even, we use the fractional part frac(t).
		// If the integer part of t is odd, we use the "mirror" of the fractional part: 1 - frac(t)
		MirroredRepeat = 2,
		// Uses the texture's border color if t is outside of [0, 1]
		ClampToBorder = 3
	};

	// A singleton/static class for configuring the global render state
	class RenderState
	{
		friend class VertexArray;
		friend class VertexBufferElement;
		friend class VertexBuffer;
		friend class IndexBuffer;
		friend class Texture1D;
		friend class Texture2D;

	public:

		// Sets background's color, used to clear window
		static void setBackgroundColor(float r, float g, float b, float a);

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

		// Enables Multisample Anti-Aliasing (MSAA) for removing jagged edges of shapes and lines.
		// IMPORTANT: In order for MSAA to work you need to have a window
		//            with numberOfSamples greater than 1, preferably 4, 8, 16, 32 or 64.
		//            You can set numberOfSamples in your WindowProperties when creating a window
		//            in your application's initialization function.
		static void enableMultisampleAntiAliasing();

		// Enables/disables face culling, which means that triangles will be rendered only if they are front-facing.
		// Back-facing triangles will not be rendered.
		static void enableFaceCulling();
		static void disableFaceCulling();
		static inline bool isEnabledFaceCulling() { return s_isEnabledFaceCulling; }

		// Returns the maximum number of texture slots supported on current hardware
		static int getMaxTextureSlots();

		// Returns the maximum supported size of a texture on current hardware.
		//
		// NOTE: This size is in texels, NOT bytes.
		// It's the maximum number of texels per dimension.
		// For example, if the maximum size is 1024,
		// this means that a 1D texture can have at most 1024 texels,
		// and a 2D texture can have at most 1024 * 1024 = 1048576 texels.
		static int getMaxTextureSize();

	private: /* functions */

		// Returns the OpenGL base data type corresponding to the given shader data type.
		// Here "base type" means that the given shader data type can be multi-component
		// and the function will return the type of a single component of that type.
		static unsigned getShaderDataTypeOpenGLBaseType(ShaderDataType type);

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

		// Returns the OpenGL enum value corresponding to the given texture minify function
		static unsigned getTextureMinifyFunctionOpenGLEnum(TextureMinifyFunction function);

		// Returns the OpenGL enum value corresponding to the given texture magnify function
		static unsigned getTextureMagnifyFunctionOpenGLEnum(TextureMagnifyFunction function);

		// Returns the OpenGL enum value corresponding to the given wrap mode
		static unsigned getTextureWrapModeOpenGLEnum(TextureWrapMode wrapMode);

	private: /* variables */

		// A flag indicating if face culling is enabled
		static bool s_isEnabledFaceCulling;
	};

} // namespace Graphics
} // namespace Pekan