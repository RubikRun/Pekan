#pragma once

#include "RenderState.h"

#include "glm/glm.hpp"

#include <string>
#include <unordered_map>

namespace Pekan {
namespace Graphics {

	// A class representing a shader program on the GPU.
	class Shader
	{
	public:

		~Shader();

		// Creates the underlying shader program object
		void create();
		// Creates the underlying shader program object with given source code for vertex shader and fragment shader
		void create(const char* vertexShaderSource, const char* fragmentShaderSource);
		void destroy();

		// Sets source code of vertex shader and fragment shader to be used for this shader program
		void setSource(const char* vertexShaderSource, const char* fragmentShaderSource);

		void bind() const;
		void unbind() const;

		/////////////////////////////////////////////////////////////////
		// Functions for setting the value of a uniform inside the shader
		/////////////////////////////////////////////////////////////////

		void setUniform1f(const char* uniformName, float value);
		void setUniform1fv(const char* uniformName, int count, const float* values);
		void setUniform1i(const char* uniformName, int value);
		void setUniform1iv(const char* uniformName, int count, const int* values);

		void setUniform2f(const char* uniformName, glm::vec2 value);
		void setUniform2fv(const char* uniformName, int count, const glm::vec2* values);
		void setUniform2i(const char* uniformName, glm::ivec2 value);
		void setUniform2iv(const char* uniformName, int count, const glm::ivec2* values);

		void setUniform3f(const char* uniformName, glm::vec3 value);
		void setUniform3fv(const char* uniformName, int count, const glm::vec3* values);
		void setUniform3i(const char* uniformName, glm::ivec3 value);
		void setUniform3iv(const char* uniformName, int count, const glm::ivec3* values);

		void setUniform4f(const char* uniformName, glm::vec4 value);
		void setUniform4fv(const char* uniformName, int count, const glm::vec4* values);
		void setUniform4i(const char* uniformName, glm::ivec4 value);
		void setUniform4iv(const char* uniformName, int count, const glm::ivec4* values);

		void setUniformMatrix4fv(const char* uniformName, const glm::mat4& value);

		// Checks if shader is valid, meaning that it has been successfully created and not yet destroyed
		bool isValid() const { return m_id != 0; }

	private: /* functions */

		// Compiles given shader's source code.
		// "Shader" here means a single shader type, for example a vertex shader.
		// It does NOT mean a whole shader program.
		// Returns compiled shader's ID
		unsigned compileShader(unsigned shaderType, const char* sourceCode);

		// Returns the location inside the shader of the uniform with the given name.
		// If such uniform doesn't exist, returns -1.
		int getUniformLocation(const std::string& uniformName) const;

		// Detaches and deletes all shaders currently attached to this shader program
		void detachAndDeleteShaders();

	private: /* variables */

		// A map that caches locations of uniforms inside the shader.
		// The uniform locations in a given shader do not change,
		// so if we need to access them repeatedly
		// it's not necessary to retrieve them with OpenGL calls every time.
		// First time we retrieve a location of a given uniform
		// we will cache it inside this map
		// and next times when we need it we can just read it from the cache.
		// It maps uniform names to uniform locations.
		mutable std::unordered_map<std::string, int> m_uniformLocationCache;

		// Flag indicating if shader program currently has any shaders attached
		bool m_hasShadersAttached = false;

		// Shader's ID on the GPU
		unsigned m_id = 0;
	};

} // namespace Pekan
} // namespace Graphics