#include "Shader.h"
#include "Logger/PekanLogger.h"
#include "PekanRenderer.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Pekan {
namespace Renderer {

	Shader::~Shader()
	{
		if (isValid())
		{
			destroy();
		}
	}

	void Shader::create(const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		// Create underlying render component, but don't bind yet.
		// We need to have a valid compiled shader program before binding.
		RenderComponent::create(false);
		// Set program's source code
		setSource(vertexShaderSource, fragmentShaderSource);
		// At this point we have compiled the shader program so we can bind shader
		bind();
	}

	void Shader::setSource(const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		// Compile shaders
		const unsigned vertexShaderID = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
		const unsigned fragmentShaderID = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
		// Attach shaders to program, and link it
		GLCall(glAttachShader(m_id, vertexShaderID));
		GLCall(glAttachShader(m_id, fragmentShaderID));
		GLCall(glLinkProgram(m_id));
		// Check if program linked successfully
		int success;
		GLCall(glGetProgramiv(m_id, GL_LINK_STATUS, &success));
		if (!success) {
			char infoLog[512];
			GLCall(glGetProgramInfoLog(m_id, 512, nullptr, infoLog));
			PK_LOG_ERROR("Shader program linking failed: " << infoLog, "Pekan");
		}
		// Delete the individual shaders, as the shader program has them now
		GLCall(glDeleteShader(vertexShaderID));
		GLCall(glDeleteShader(fragmentShaderID));
	}

	void Shader::bind() const {
		GLCall(glUseProgram(m_id));
	}

	void Shader::unbind() const {
		GLCall(glUseProgram(0));
	}

	void Shader::setUniform1f(const char* uniformName, float value)
	{
		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform1f(location, value));
	}

	void Shader::setUniform1i(const char* uniformName, int value)
	{
		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform1i(location, value));
	}

	void Shader::setUniform2fv(const char* uniformName, const glm::vec2& value)
	{
		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform2fv(location, 1, glm::value_ptr(value)));
	}

	void Shader::setUniform3fv(const char* uniformName, const glm::vec3& value)
	{
		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform3fv(location, 1, glm::value_ptr(value)));
	}

	void Shader::setUniform4fv(const char* uniformName, const glm::vec4& value)
	{
		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform4fv(location, 1, glm::value_ptr(value)));
	}

	void Shader::setUniformMatrix4fv(const char* uniformName, const glm::mat4& value)
	{
		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]));
	}

	void Shader::_create()
	{
		GLCall(m_id = glCreateProgram());
	}

	void Shader::_destroy()
	{
		GLCall(glDeleteProgram(m_id));
		// Clear the cache of uniform locations
		// since they apply specifically to the shader being destroyed here.
		m_uniformLocationCache.clear();
	}

	unsigned Shader::compileShader(unsigned shaderType, const char* sourceCode) {
		GLCall(const unsigned shaderID = glCreateShader(shaderType));
		GLCall(glShaderSource(shaderID, 1, &sourceCode, nullptr));
		GLCall(glCompileShader(shaderID));

		int success;
		GLCall(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success));
		if (!success) {
			char infoLog[512];
			GLCall(glGetShaderInfoLog(shaderID, 512, nullptr, infoLog));
			PK_LOG_ERROR("Shader compilation failed: " << infoLog, "Pekan");
		}
		return shaderID;
	}

	int Shader::getUniformLocation(const std::string& uniformName) const {
		// If we have the location of this uniform cached, retrieve it from cache
		const auto cacheIt = m_uniformLocationCache.find(uniformName);
		if (cacheIt != m_uniformLocationCache.end()) {
			return cacheIt->second;
		}
		// Otherwise retrieve it by asking OpenGL for the location
		GLCall(const int location = glGetUniformLocation(m_id, uniformName.c_str()));
		if (location < 0) {
			PK_LOG_ERROR("Trying to set value for uniform \"" << uniformName << "\" inside a shader, but such uniform doesn't exist.", "Pekan");
		}
		// Cache the location so that it can be reused in next calls to this function
		m_uniformLocationCache[uniformName] = location;
		return location;
	}

} // namespace Pekan
} // namespace Renderer