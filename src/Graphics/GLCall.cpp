#include "GLCall.h"

namespace Pekan
{
namespace Graphics
{

	std::string _getGLErrorMessage(unsigned error)
	{
		switch (error)
		{
			case GL_NO_ERROR:                         return "No error";
			case GL_INVALID_ENUM:                     return "Invalid enum";
			case GL_INVALID_VALUE:                    return "Invalid value";
			case GL_INVALID_OPERATION:                return "Invalid operation";
			case GL_STACK_OVERFLOW:                   return "Stack overflow";
			case GL_STACK_UNDERFLOW:                  return "Stack underflow";
			case GL_OUT_OF_MEMORY:                    return "Out of memory";
			case GL_INVALID_FRAMEBUFFER_OPERATION:    return "Invalid framebuffer operation";
		}
		return "Unknown error";
	}

} // namespace Graphics
} // namespace Pekan
