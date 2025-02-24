#include "DemoScene.h"
#define PK_FILENAME "DemoScene.cpp"
#include "Logger/PekanLogger.h"
#include "Utils/PekanUtils.h"

using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::VertexBufferElement;
using Pekan::Renderer::VertexBufferLayout;
using Pekan::Renderer::ShaderDataType;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static const char* vertexShaderFilePath = "resources/vertex_shader.glsl";
static const char* fragmentShaderFilePath = "resources/fragment_shader.glsl";

namespace Demo
{

	void DemoScene::update()
	{
        shader.bind();
        const ImVec2& position = guiWindow->getPosition();
        shader.setUniform2fv("uPosition", glm::vec2(position.x, position.y));
        shader.unbind();
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
        indexBuffer.destroy();
        vertexArray.destroy();
        shader.destroy();
	}

	bool DemoScene::_init()
	{
        shader.create(
            Pekan::Utils::readFileToString(vertexShaderFilePath).c_str(),
            Pekan::Utils::readFileToString(fragmentShaderFilePath).c_str()
        );

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

        // Create a vertex array
        vertexArray.create();

        // Create a vertex buffer with vertices data
        vertexBuffer.create(vertices, sizeof(vertices), { { ShaderDataType::Float2, "position" }, { ShaderDataType::Float4, "color" } });
        // Create an index buffer with indices data
        indexBuffer.create(indices, sizeof(indices));

        // Add vertex buffer to vertex array
        vertexArray.addVertexBuffer(vertexBuffer);

        return true;
	}

} // namespace Demo