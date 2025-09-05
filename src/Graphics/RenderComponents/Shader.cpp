#include "Shader.h"

#include "PekanLogger.h"
#include "GLCall.h"
#include "RenderState.h"

#include <glm/gtc/type_ptr.hpp>

namespace Pekan {
namespace Graphics {

	static const unsigned MAX_SHADERS_ATTACHED = 10;

	Shader::~Shader()
	{
		PK_ASSERT(!isValid(), "You forgot to destroy() a Shader instance.", "Pekan");
	}

	void Shader::create()
	{
		PK_ASSERT(!isValid(), "Trying to create a Shader instance that is already created.", "Pekan");

		m_hasShadersAttached = false;
		GLCall(m_id = glCreateProgram());
	}

	void Shader::create(const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		PK_ASSERT(!isValid(), "Trying to create a Shader instance that is already created.", "Pekan");

		m_hasShadersAttached = false;
		GLCall(m_id = glCreateProgram());
		setSource(vertexShaderSource, fragmentShaderSource);
		// We can bind the shader once we set its source
		bind();
	}

	void Shader::destroy()
	{
		PK_ASSERT(isValid(), "Trying to destroy a Shader instance that is not yet created.", "Pekan");

		GLCall(glDeleteProgram(m_id));
		m_id = 0;

		m_hasShadersAttached = false;
		// Clear the cache of uniform locations
		// since they apply specifically to the shader being destroyed here.
		m_uniformLocationCache.clear();
	}

	void Shader::setSource(const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		PK_ASSERT(isValid(), "Trying to set source to a Shader that is not yet created.", "Pekan");

		if (m_hasShadersAttached)
		{
			detachAndDeleteShaders();
			m_uniformLocationCache.clear();
		}

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

		m_hasShadersAttached = true;
	}

	void Shader::bind() const {
		PK_ASSERT(isValid(), "Trying to bind a Shader that is not yet created.", "Pekan");

		GLCall(glUseProgram(m_id));
	}

	void Shader::unbind() const {
		PK_ASSERT(isValid(), "Trying to unbind a Shader that is not yet created.", "Pekan");

		GLCall(glUseProgram(0));
	}

	void Shader::setUniform1f(const char* uniformName, float value)
	{
		PK_ASSERT(isValid(), "Trying to set a uniform1f to a Shader that is not yet created.", "Pekan");

		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform1f(location, value));
	}

	void Shader::setUniform1fv(const char* uniformName, int count, const float* values)
	{
		PK_ASSERT(isValid(), "Trying to set a uniform1fv to a Shader that is not yet created.", "Pekan");
		if (values == nullptr)
		{
			PK_LOG_ERROR("Trying to set a uniform1fv to a Shader but given values pointer is null.", "Pekan");
			return;
		}

		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform1fv(location, count, values));
	}

	void Shader::setUniform1i(const char* uniformName, int value)
	{
		PK_ASSERT(isValid(), "Trying to set a uniform1i to a Shader that is not yet created.", "Pekan");

		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform1i(location, value));
	}

	void Shader::setUniform1iv(const char* uniformName, int count, const int* values)
	{
		PK_ASSERT(isValid(), "Trying to set a uniform1iv to a Shader that is not yet created.", "Pekan");
		if (values == nullptr)
		{
			PK_LOG_ERROR("Trying to set a uniform1iv to a Shader but given values pointer is null.", "Pekan");
			return;
		}

		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform1iv(location, count, values));
	}

	void Shader::setUniform2f(const char* uniformName, glm::vec2 value)
	{
		PK_ASSERT(isValid(), "Trying to set a uniform2f to a Shader that is not yet created.", "Pekan");

		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform2f(location, value.x, value.y));
	}

	void Shader::setUniform2fv(const char* uniformName, int count, const glm::vec2* values)
	{
		PK_ASSERT(isValid(), "Trying to set a uniform2fv to a Shader that is not yet created.", "Pekan");
		if (values == nullptr)
		{
			PK_LOG_ERROR("Trying to set a uniform2fv to a Shader but given values pointer is null.", "Pekan");
			return;
		}

		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform2fv(location, count, glm::value_ptr(*values)));
	}

	void Shader::setUniform2i(const char* uniformName, glm::ivec2 value)
	{
		PK_ASSERT(isValid(), "Trying to set a uniform2i to a Shader that is not yet created.", "Pekan");

		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform2i(location, value.x, value.y));
	}

	void Shader::setUniform2iv(const char* uniformName, int count, const glm::ivec2* values)
	{
		PK_ASSERT(isValid(), "Trying to set a uniform2iv to a Shader that is not yet created.", "Pekan");
		if (values == nullptr)
		{
			PK_LOG_ERROR("Trying to set a uniform2iv to a Shader but given values pointer is null.", "Pekan");
			return;
		}

		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform2iv(location, count, glm::value_ptr(*values)));
	}

	void Shader::setUniform3f(const char* uniformName, glm::vec3 value)
	{
		PK_ASSERT(isValid(), "Trying to set a uniform3f to a Shader that is not yet created.", "Pekan");

		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform3f(location, value.x, value.y, value.z));
	}

	void Shader::setUniform3fv(const char* uniformName, int count, const glm::vec3* values)
	{
		PK_ASSERT(isValid(), "Trying to set a uniform3fv to a Shader that is not yet created.", "Pekan");
		if (values == nullptr)
		{
			PK_LOG_ERROR("Trying to set a uniform3fv to a Shader but given values pointer is null.", "Pekan");
			return;
		}

		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform3fv(location, count, glm::value_ptr(*values)));
	}

	void Shader::setUniform3i(const char* uniformName, glm::ivec3 value)
	{
		PK_ASSERT(isValid(), "Trying to set a uniform3i to a Shader that is not yet created.", "Pekan");

		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform3i(location, value.x, value.y, value.z));
	}

	void Shader::setUniform3iv(const char* uniformName, int count, const glm::ivec3* values)
	{
		PK_ASSERT(isValid(), "Trying to set a uniform3iv to a Shader that is not yet created.", "Pekan");
		if (values == nullptr)
		{
			PK_LOG_ERROR("Trying to set a uniform3iv to a Shader but given values pointer is null.", "Pekan");
			return;
		}

		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform3iv(location, count, glm::value_ptr(*values)));
	}

	void Shader::setUniform4f(const char* uniformName, glm::vec4 value)
	{
		PK_ASSERT(isValid(), "Trying to set a uniform4f to a Shader that is not yet created.", "Pekan");

		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform4f(location, value.x, value.y, value.z, value.w));
	}

	void Shader::setUniform4fv(const char* uniformName, int count, const glm::vec4* values)
	{
		PK_ASSERT(isValid(), "Trying to set a uniform4fv to a Shader that is not yet created.", "Pekan");
		if (values == nullptr)
		{
			PK_LOG_ERROR("Trying to set a uniform4fv to a Shader but given values pointer is null.", "Pekan");
			return;
		}

		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform4fv(location, count, glm::value_ptr(*values)));
	}

	void Shader::setUniform4i(const char* uniformName, glm::ivec4 value)
	{
		PK_ASSERT(isValid(), "Trying to set a uniform4i to a Shader that is not yet created.", "Pekan");

		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform4i(location, value.x, value.y, value.z, value.w));
	}

	void Shader::setUniform4iv(const char* uniformName, int count, const glm::ivec4* values)
	{
		PK_ASSERT(isValid(), "Trying to set a uniform4iv to a Shader that is not yet created.", "Pekan");
		if (values == nullptr)
		{
			PK_LOG_ERROR("Trying to set a uniform4iv to a Shader but given values pointer is null.", "Pekan");
			return;
		}

		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniform4iv(location, count, glm::value_ptr(*values)));
	}

	void Shader::setUniformMatrix4fv(const char* uniformName, const glm::mat4& value)
	{
		PK_ASSERT(isValid(), "Trying to set a uniformMatrix4fv to a Shader that is not yet created.", "Pekan");

		bind();
		const int location = getUniformLocation(uniformName);
		GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]));
	}

	unsigned Shader::compileShader(unsigned shaderType, const char* sourceCode) {
		PK_ASSERT(isValid(), "Trying to compile a Shader that is not yet created.", "Pekan");

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
		PK_ASSERT(isValid(), "Trying to get uniform location from a Shader that is not yet created.", "Pekan");

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

	void Shader::detachAndDeleteShaders()
	{
		PK_ASSERT(isValid(), "Trying to detach and delete a Shader that is not yet created.", "Pekan");

		int shaderCount = 0;
		unsigned shaders[MAX_SHADERS_ATTACHED];
		// Get attached shaders
		glGetAttachedShaders(m_id, MAX_SHADERS_ATTACHED, &shaderCount, shaders);
		// Detach and delete each shader
		for (int i = 0; i < shaderCount; i++)
		{
			glDetachShader(m_id, shaders[i]);
			glDeleteShader(shaders[i]);
		}

		m_hasShadersAttached = false;
	}

} // namespace Pekan
} // namespace Graphics