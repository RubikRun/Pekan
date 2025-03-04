#include "Shader.h"
#include "Logger/PekanLogger.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Pekan {
namespace Renderer {

	Shader::~Shader() {
		if (isValid())
		{
			destroy();
		}
	}

	void Shader::create(const char* vertexShaderSource, const char* fragmentShaderSource) {
		// Compile shaders
		const GLuint vertexShaderID = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
		const GLuint fragmentShaderID = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
		// Create program, attach shaders to it, and link it
		m_id = glCreateProgram();
		glAttachShader(m_id, vertexShaderID);
		glAttachShader(m_id, fragmentShaderID);
		glLinkProgram(m_id);
		// Check if program linked successfully
		GLint success;
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
			PK_LOG_ERROR("Shader program linking failed: " << infoLog);
		}
		// Delete the individual shaders, as the shader program has them now
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	void Shader::destroy() {
		if (isValid())
		{
			unbind();
			glDeleteProgram(m_id);
			m_id = 0;
			// Clear the cache of uniform locations
			// since they apply specifically to the shader being destroyed here.
			m_uniformLocationCache.clear();
		}
	}

	void Shader::bind() const {
		glUseProgram(m_id);
	}

	void Shader::unbind() const {
		glUseProgram(0);
	}

	void Shader::setUniform1f(const char* uniformName, float value) {
		const GLint location = getUniformLocation(uniformName);
		glUniform1f(location, value);
	}

	void Shader::setUniform2fv(const char* uniformName, const glm::vec2& value)
	{
		const GLint location = getUniformLocation(uniformName);
		glUniform2fv(location, 1, glm::value_ptr(value));
	}

	void Shader::setUniform3fv(const char* uniformName, const glm::vec3& value) {
		const GLint location = getUniformLocation(uniformName);
		glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void Shader::setUniformMatrix4fv(const char* uniformName, const glm::mat4& value) {
		const GLint location = getUniformLocation(uniformName);
		glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
	}

	unsigned Shader::compileShader(unsigned shaderType, const char* sourceCode) {
		const GLuint shaderID = glCreateShader(shaderType);
		glShaderSource(shaderID, 1, &sourceCode, nullptr);
		glCompileShader(shaderID);

		GLint success;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
			PK_LOG_ERROR("Shader compilation failed: " << infoLog);
		}
		return shaderID;
	}

	GLint Shader::getUniformLocation(const std::string& uniformName) const {
		// If we have the location of this uniform cached, retrieve it from cache
		const auto cacheIt = m_uniformLocationCache.find(uniformName);
		if (cacheIt != m_uniformLocationCache.end()) {
			return cacheIt->second;
		}
		// Otherwise retrieve it by asking OpenGL for the location
		const GLint location = glGetUniformLocation(m_id, uniformName.c_str());
		if (location < 0) {
			PK_LOG_ERROR("Trying to set value for uniform \"" << uniformName << "\" inside a shader, but such uniform doesn't exist.");
		}
		// Cache the location so that it can be reused in next calls to this function
		m_uniformLocationCache[uniformName] = location;
		return location;
	}

} // namespace Pekan
} // namespace Renderer