#include "PekanRenderer.h"
#define PK_FILENAME "PekanRenderer.cpp"
#include "Logger/PekanLogger.h"

#include <glad/glad.h>

namespace Pekan
{
namespace Renderer
{

	void PekanRenderer::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void PekanRenderer::setBackgroundColor(const glm::vec4& backgroundColor)
	{
		glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
	}

	GLenum PekanRenderer::getShaderDataTypeOpenGLBaseType(ShaderDataType type)
	{
		switch (type) {
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
			case ShaderDataType::Bool:     return GL_BOOL;
		}
		PK_LOG_ERRORF("Unknown ShaderDataType, cannot determine OpenGL base type.");
		return 0;
	}

	unsigned PekanRenderer::getShaderDataTypeSize(ShaderDataType type)
	{
		switch (type) {
			case ShaderDataType::Float:    return 4;
			case ShaderDataType::Float2:   return 4 * 2;
			case ShaderDataType::Float3:   return 4 * 3;
			case ShaderDataType::Float4:   return 4 * 4;
			case ShaderDataType::Mat3:     return 4 * 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4 * 4;
			case ShaderDataType::Int:      return 4;
			case ShaderDataType::Int2:     return 4 * 2;
			case ShaderDataType::Int3:     return 4 * 3;
			case ShaderDataType::Int4:     return 4 * 4;
			case ShaderDataType::Bool:     return 1;
		}
		PK_LOG_ERRORF("Unknown ShaderDataType, cannot determine its size.");
		return 0;
	}

	unsigned PekanRenderer::getShaderDataTypeComponentsCount(ShaderDataType type)
	{
		switch (type) {
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3 * 3;
			case ShaderDataType::Mat4:    return 4 * 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
		}
		PK_LOG_ERRORF("Unknown ShaderDataType, cannot determine its components count.");
		return 0;
	}

} // namespace Renderer
} // namespace Pekan