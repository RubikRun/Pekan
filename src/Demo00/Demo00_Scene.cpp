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

    bool Demo00_Scene::init()
	{
        m_shader.create(
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
        m_vertexArray.create();

        // Create a vertex buffer with vertices data
        m_vertexBuffer.create(
            vertices,
            sizeof(vertices)
        );
        // Create an index buffer with indices data
        m_indexBuffer.create(indices, sizeof(indices));

        // Add vertex buffer to vertex array
        m_vertexArray.addVertexBuffer(m_vertexBuffer, { { ShaderDataType::Float2, "position" }, { ShaderDataType::Float4, "color" } });

        return true;
	}

	void Demo00_Scene::update()
	{
        m_shader.bind();
        const ImVec2& position = m_guiWindow->getPosition();
        m_shader.setUniform2fv("uPosition", glm::vec2(position.x, position.y));
        m_shader.unbind();
	}

	void Demo00_Scene::render()
	{
        // Clear background color
        if (m_guiWindow != nullptr)
        {
            const ImVec4& clearColor = m_guiWindow->getClearColor();
            PekanRenderer::setBackgroundColor(glm::vec4(clearColor.x, clearColor.y, clearColor.z, clearColor.w));
            PekanRenderer::clear();
        }
        else
        {
            // If there is no GUI window attached, just clear background with default color
            PekanRenderer::clear();
        }

        m_shader.bind();
        m_vertexArray.bind();
        PekanRenderer::drawIndexed(6, DrawMode::Triangles);
        m_vertexArray.unbind();
        m_shader.unbind();
	}

	void Demo00_Scene::exit()
	{
        m_vertexBuffer.destroy();
        m_indexBuffer.destroy();
        m_vertexArray.destroy();
        m_shader.destroy();
	}

} // namespace Demo