#include "Demo00_Scene.h"
#include "PekanLogger.h"
#include "Utils/FileUtils.h"

constexpr char* const vertexShaderFilePath = "resources/00_vertex_shader.glsl";
constexpr char* const fragmentShaderFilePath = "resources/00_fragment_shader.glsl";

using namespace Pekan::Graphics;

namespace Demo
{

    bool Demo00_Scene::init()
	{
        // Set up vertex data and configure vertex attributes
        constexpr float vertices[] =
        {
             0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, // top right
             0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
            -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f // top left
        };
        constexpr unsigned indices[] =
        {
            0, 1, 3,  // first triangle
            1, 2, 3   // second triangle
        };

        m_renderObject.create
        (
            vertices, sizeof(vertices),
            { { ShaderDataType::Float2, "position" }, { ShaderDataType::Float4, "color" } },
            BufferDataUsage::StaticDraw,
            Pekan::FileUtils::readTextFileToString(vertexShaderFilePath).c_str(),
            Pekan::FileUtils::readTextFileToString(fragmentShaderFilePath).c_str()
        );
        m_renderObject.setIndexData(indices, sizeof(indices), BufferDataUsage::StaticDraw);

        return true;
	}

	void Demo00_Scene::update(double dt)
	{
        // Get position from GUI
        glm::vec2 position = m_guiWindow->getPosition();
        // Set "uPosition" uniform inside of the shader
        Shader& shader = m_renderObject.getShader();
        shader.bind();
        shader.setUniform2f("uPosition", position);
        shader.unbind();
	}

	void Demo00_Scene::render() const
	{
        // Clear background color
        if (m_guiWindow != nullptr)
        {
            glm::vec4 clearColor = m_guiWindow->getClearColor();
            RenderState::setBackgroundColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        }
        RenderCommands::clear();

        m_renderObject.render();
	}

	void Demo00_Scene::exit()
	{
        m_renderObject.destroy();
	}

} // namespace Demo