#include "Demo00_Scene.h"
#include "Logger/PekanLogger.h"
#include "Utils/PekanUtils.h"

using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::ShaderDataType;
using Pekan::Renderer::DrawMode;
using Pekan::Renderer::BufferDataUsage;
using Pekan::Renderer::Shader;

static const char* vertexShaderFilePath = "resources/00_vertex_shader.glsl";
static const char* fragmentShaderFilePath = "resources/00_fragment_shader.glsl";

namespace Demo
{

    bool Demo00_Scene::init()
	{
        // Set up vertex data and configure vertex attributes
        const float vertices[] =
        {
             0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, // top right
             0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
            -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f // top left
        };
        const unsigned indices[] =
        {
            0, 1, 3,  // first triangle
            1, 2, 3   // second triangle
        };

        m_renderObject.create
        (
            vertices, sizeof(vertices),
            { { ShaderDataType::Float2, "position" }, { ShaderDataType::Float4, "color" } },
            BufferDataUsage::StaticDraw,
            indices, sizeof(indices),
            BufferDataUsage::StaticDraw,
            Pekan::Utils::readFileToString(vertexShaderFilePath).c_str(),
            Pekan::Utils::readFileToString(fragmentShaderFilePath).c_str()
        );

        return true;
	}

	void Demo00_Scene::update(double dt)
	{
        // Get position from GUI
        const ImVec2& position = m_guiWindow->getPosition();
        // Set "uPosition" uniform inside of the shader
        Shader& shader = m_renderObject.getShader();
        shader.bind();
        shader.setUniform2fv("uPosition", glm::vec2(position.x, position.y));
        shader.unbind();
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

        m_renderObject.bind();
        PekanRenderer::drawIndexed(6, DrawMode::Triangles);
        m_renderObject.unbind();
	}

	void Demo00_Scene::exit()
	{
        m_renderObject.destroy();
	}

} // namespace Demo