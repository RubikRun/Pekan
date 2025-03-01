#include "Demo00_Scene.h"
#define PK_FILENAME "DemoScene.cpp"
#include "Logger/PekanLogger.h"
#include "Utils/PekanUtils.h"

using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::VertexBufferElement;
using Pekan::Renderer::VertexBufferLayout;
using Pekan::Renderer::ShaderDataType;
using Pekan::Renderer::DrawMode;

static const char* vertexShaderFilePath = "resources/00_vertex_shader.glsl";
static const char* fragmentShaderFilePath = "resources/00_fragment_shader.glsl";

namespace Demo
{

	void Demo00_Scene::update()
	{
        shader.bind();
        const ImVec2& position = guiWindow->getPosition();
        shader.setUniform2fv("uPosition", glm::vec2(position.x, position.y));
        shader.unbind();
	}

	void Demo00_Scene::render()
	{
        // Clear background color
        if (guiWindow != nullptr)
        {
            const ImVec4& clearColor = guiWindow->getClearColor();
            PekanRenderer::setBackgroundColor(glm::vec4(clearColor.x, clearColor.y, clearColor.z, clearColor.w));
            PekanRenderer::clear();
        }
        else
        {
            // If there is no GUI window attached, just clear background with default color
            PekanRenderer::clear();
        }

        shader.bind();
        vertexArray.bind();
        PekanRenderer::drawIndexed(6, DrawMode::Triangles);
        vertexArray.unbind();
        shader.unbind();
	}

	void Demo00_Scene::exit()
	{
        vertexBuffer.destroy();
        indexBuffer.destroy();
        vertexArray.destroy();
        shader.destroy();
	}

	bool Demo00_Scene::_init()
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
        vertexBuffer.create(
            vertices,
            sizeof(vertices)
        );
        // Create an index buffer with indices data
        indexBuffer.create(indices, sizeof(indices));

        // Add vertex buffer to vertex array
        vertexArray.addVertexBuffer(vertexBuffer, { { ShaderDataType::Float2, "position" }, { ShaderDataType::Float4, "color" } });

        return true;
	}

} // namespace Demo