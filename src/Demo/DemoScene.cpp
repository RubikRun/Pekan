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

        shader.bind();
        vertexArray.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        vertexArray.unbind();
        shader.unbind();
	}

	void DemoScene::exit()
	{
        vertexBuffer.destroy();
        vertexArray.destroy();
        shader.destroy();

        glDeleteBuffers(1, &ebo);
	}

	bool DemoScene::_init()
	{
        shader.create(vertexShaderSource, fragmentShaderSource);

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
        // Create a vertex buffer with vertices data
        vertexBuffer.create(vertices, sizeof(vertices), { { ShaderDataType::Float2, "position" }, { ShaderDataType::Float4, "color" } });
        // Create a vertex array. Add vertex buffer to it.
        vertexArray.create();
        vertexArray.addVertexBuffer(vertexBuffer);

        // Create an element buffer object
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        // Fill element buffer object with indices data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        return true;
	}

} // namespace Demo