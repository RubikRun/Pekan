#include "DemoScene.h"
#define PK_FILENAME "DemoScene.cpp"
#include "Logger/PekanLogger.h"

using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::VertexBufferElement;
using Pekan::Renderer::VertexBufferLayout;
using Pekan::Renderer::ShaderDataType;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"layout (location = 1) in vec4 aColor;\n"
"out vec4 vColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
"   vColor = aColor;\n"
"}\0";

static const char* fragmentShaderSource = "#version 330 core\n"
"in vec4 vColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vColor;\n"
"}\n\0";

namespace Demo
{

	void DemoScene::update()
	{
	}

	void DemoScene::render()
	{
        // Clear background color
        if (guiWindow != nullptr)
        {
            const ImVec4& clearColor = guiWindow->getClearColor();
            glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        else
        {
            // If there is no GUI window attached, just clear background with black
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
	}

	void DemoScene::exit()
	{
        vertexBuffer.destroy();

        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &ebo);
        glDeleteProgram(shaderProgram);
	}

	bool DemoScene::_init()
	{
        // Compile vertex shader
        unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        // Check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            PK_LOG_ERRORF("ERROR: Vertex shader failed to compile with the following error: " << infoLog);
            return false;
        }
        // Compile fragment shader
        unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        // Check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            PK_LOG_ERRORF("ERROR: Fragment shader failed to compile with the following error: " << infoLog);
            return false;
        }
        // Link shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // Check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            PK_LOG_ERRORF("ERROR: Shader program failed to link with the following error: " << infoLog);
            return false;
        }
        // Delete vertex shader and fragment shader, as they are already linked into a shader program
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Set up vertex data and configure vertex attributes
        float vertices[] = {
             0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, // top right
             0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
            -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f // top left
        };
        unsigned indices[] = {
            0, 1, 3,  // first triangle
            1, 2, 3   // second triangle
        };
        // Create a vertex array object, and an element buffer object
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &ebo);
        // Bind vertex array object and element buffer object
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        // Create a vertex buffer with vertices data
        vertexBuffer.create(vertices, sizeof(vertices));
        // Fill element buffer object with indices data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        // Configure vertex attributes
        const VertexBufferLayout layout({
            { ShaderDataType::Float2, "position" },
            { ShaderDataType::Float4, "color" },
        });
        const std::vector<VertexBufferElement>& layoutElements = layout.getElements();
        for (int i = 0; i < layoutElements.size(); ++i) {
            const VertexBufferElement& element = layoutElements[i];
            glVertexAttribPointer(
                i,
                element.getComponentsCount(),
                PekanRenderer::getShaderDataTypeOpenGLBaseType(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                layout.getStride(),
                reinterpret_cast<void*>((long long unsigned)(element.getOffset()))
            );
            glEnableVertexAttribArray(i);
        }

        return true;
	}

} // namespace Demo