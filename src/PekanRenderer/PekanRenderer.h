#pragma once

#include <glad/glad.h>

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

	class PekanRenderer
	{
	public:

		// Returns the OpenGL base data type corresponding to the given shader data type.
		// Here "base type" means that the given shader data type can be multi-component
		// and the function will return the type of a single component of that type.
		static GLenum getShaderDataTypeOpenGLBaseType(ShaderDataType type);

		// Returns size in bytes of a given shader data type
		static unsigned getShaderDataTypeSize(ShaderDataType type);

		// Returns number of components of a given shader data type
		static unsigned getShaderDataTypeComponentsCount(ShaderDataType type);
	};

} // namespace Renderer
} // namespace Pekan