#include "Demo02_Scene.h"
#include "Utils/PekanUtils.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::ShaderDataType;
using Pekan::PekanEngine;

#define PI 3.14159265359f

static const char* vertexShaderFilePath = "resources/02_vertex_shader.glsl";
static const char* fragmentShaderFilePath = "resources/02_fragment_shader.glsl";

static const float COLOR_DULLNESS = 0.9f;

namespace Demo
{

    const unsigned indices[] =
    {
        0, 1, 2, 2, 3, 0, // Back
        4, 5, 6, 6, 7, 4, // Front
        8, 9, 10, 10, 11, 8, // Left
        12, 13, 14, 14, 15, 12, // Right
        16, 17, 18, 18, 19, 16, // Top
        20, 21, 22, 22, 23, 20  // Bottom
    };

    bool Demo02_Scene::init()
	{
        m_resolution = PekanEngine::getWindowResolution();

        // Set default colors of cube's sides
        m_colors =
        {
            {COLOR_DULLNESS, 0.0f, 0.0f},
            {0.0f, COLOR_DULLNESS, 0.0f},
            {0.0f, 0.0f, COLOR_DULLNESS},
            {COLOR_DULLNESS, COLOR_DULLNESS, 0.0f},
            {COLOR_DULLNESS, 0.0f, COLOR_DULLNESS},
            {0.0f, COLOR_DULLNESS, COLOR_DULLNESS}
        };

        // Initialize vertices of cube
        m_vertices =
        {
            // Back face (Red)
            {{-0.5f, -0.5f, -0.5f}, m_colors[0]},
            {{ 0.5f, -0.5f, -0.5f}, m_colors[0]},
            {{ 0.5f,  0.5f, -0.5f}, m_colors[0]},
            {{-0.5f,  0.5f, -0.5f}, m_colors[0]},

            // Front face (Green)
            {{-0.5f, -0.5f,  0.5f}, m_colors[1]},
            {{ 0.5f, -0.5f,  0.5f}, m_colors[1]},
            {{ 0.5f,  0.5f,  0.5f}, m_colors[1]},
            {{-0.5f,  0.5f,  0.5f}, m_colors[1]},

            // Left face (Blue)
            {{-0.5f, -0.5f, -0.5f}, m_colors[2]},
            {{-0.5f,  0.5f, -0.5f}, m_colors[2]},
            {{-0.5f,  0.5f,  0.5f}, m_colors[2]},
            {{-0.5f, -0.5f,  0.5f}, m_colors[2]},

            // Right face (Yellow)
            {{ 0.5f, -0.5f, -0.5f}, m_colors[3]},
            {{ 0.5f,  0.5f, -0.5f}, m_colors[3]},
            {{ 0.5f,  0.5f,  0.5f}, m_colors[3]},
            {{ 0.5f, -0.5f,  0.5f}, m_colors[3]},

            // Top face (Magenta)
            {{-0.5f,  0.5f, -0.5f}, m_colors[4]},
            {{ 0.5f,  0.5f, -0.5f}, m_colors[4]},
            {{ 0.5f,  0.5f,  0.5f}, m_colors[4]},
            {{-0.5f,  0.5f,  0.5f}, m_colors[4]},

            // Bottom face (Cyan)
            {{-0.5f, -0.5f, -0.5f}, m_colors[5]},
            {{ 0.5f, -0.5f, -0.5f}, m_colors[5]},
            {{ 0.5f, -0.5f,  0.5f}, m_colors[5]},
            {{-0.5f, -0.5f,  0.5f}, m_colors[5]}
        };

        PekanRenderer::enableDepthTest();

        // Create a vertex array
        m_vertexArray.create();

        // Create a vertex buffer with vertices data
        m_vertexBuffer.create
        (
            m_vertices.data(),
            m_vertices.size() * sizeof(Vertex)
        );

        // Add vertex buffer to vertex array
        m_vertexArray.addVertexBuffer(m_vertexBuffer, { { ShaderDataType::Float3, "position" }, { ShaderDataType::Float3, "color" } });

        // Create an index buffer with indices data
        m_indexBuffer.create(indices, sizeof(indices));

        // Create a shader by compiling source code of vertex shader and fragment shader
        m_shader.create
        (
            Pekan::Utils::readFileToString(vertexShaderFilePath).c_str(),
            Pekan::Utils::readFileToString(fragmentShaderFilePath).c_str()
        );

        // Set default transform matrices
        m_modelMatrix = glm::rotate(glm::mat4(1.0f), m_rotation * PI / 180.0f, glm::vec3(0.5f, 1.0f, 0.0f));
        m_viewMatrix = glm::lookAt(glm::vec3(m_cameraDist, m_cameraDist, m_cameraDist), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        m_projMatrix = glm::perspective(glm::radians(m_fov), float(m_resolution.x) / float(m_resolution.y), 0.01f, 100.0f);

        m_rotation = 0.0f;

        return true;
	}

	void Demo02_Scene::update()
	{
        // Set model, view, and projection matrices with current properties
        m_modelMatrix = glm::rotate(glm::mat4(1.0f), m_rotation * PI / 180.0f, glm::vec3(0.5f, 1.0f, 0.0f));
        m_viewMatrix = glm::lookAt(glm::vec3(m_cameraDist, m_cameraDist, m_cameraDist), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        m_projMatrix = glm::perspective(glm::radians(m_fov), float(m_resolution.x) / float(m_resolution.y), 0.01f, 100.0f);

        for (int i = 0; i < m_colors.size(); i++)
        {
            m_vertices[i * 4 + 0].color = m_colors[i];
            m_vertices[i * 4 + 1].color = m_colors[i];
            m_vertices[i * 4 + 2].color = m_colors[i];
            m_vertices[i * 4 + 3].color = m_colors[i];
        }

        m_vertexArray.bind();
        m_vertexBuffer.bind();
        m_vertexBuffer.setData
        (
            m_vertices.data(),
            m_vertices.size() * sizeof(Vertex)
        );

        m_rotation += 1.0f;
	}

	void Demo02_Scene::render()
	{
        PekanRenderer::clear(true);

        m_vertexBuffer.bind();
        m_vertexArray.bind();
        m_indexBuffer.bind();
        m_shader.bind();

        // Set uniform of model-view-projection matrix
        glm::mat4 mvpMatrix = m_projMatrix * m_viewMatrix * m_modelMatrix;
        m_shader.setUniformMatrix4fv("u_MVP", mvpMatrix);

        // Draw cube
        PekanRenderer::drawIndexed(36);

        m_vertexBuffer.unbind();
        m_vertexArray.unbind();
        m_indexBuffer.unbind();
        m_shader.unbind();
	}

	void Demo02_Scene::exit()
	{
        m_vertexBuffer.destroy();
        m_vertexArray.destroy();
        m_indexBuffer.destroy();
        m_shader.destroy();
	}

} // namespace Demo