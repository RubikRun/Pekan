#include "Demo00_Scene.h"
#include "PekanLogger.h"
#include "Utils/FileUtils.h"
#include "RenderCommands.h"

static const char* vertexShaderFilePath = "resources/00_vertex_shader.glsl";
static const char* fragmentShaderFilePath = "resources/00_fragment_shader.glsl";

using namespace Pekan::Graphics;

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
        shader.setUniform2fv("uPosition", position);
        shader.unbind();
	}

	void Demo00_Scene::render() const
	{
        // Clear background color
        if (m_guiWindow != nullptr)
        {
            glm::vec4 clearColor = m_guiWindow->getClearColor();
            RenderState::setBackgroundColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
            RenderCommands::clear();
        }
        else
        {
            // If there is no GUI window attached, just clear background with default color
            RenderCommands::clear();
        }

        m_renderObject.bind();
        RenderCommands::drawIndexed(6, DrawMode::Triangles);
        m_renderObject.unbind();
	}

	void Demo00_Scene::exit()
	{
        m_renderObject.destroy();
	}

} // namespace Demo