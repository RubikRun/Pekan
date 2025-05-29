#include "Demo02_Scene.h"
#include "Utils/PekanUtils.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::ShaderDataType;
using Pekan::Renderer::BufferDataUsage;
using Pekan::PekanEngine;

#define PI 3.14159265359f

static const char* vertexShaderFilePath = "resources/02_vertex_shader.glsl";
static const char* fragmentShaderFilePath = "resources/02_fragment_shader.glsl";

static const float COLOR_DULLNESS = 0.9f;

namespace Demo
{

    const unsigned indices[] =
    {
        0, 1, 2, 0, 2, 3, // Back
        4, 5, 6, 4, 6, 7, // Front
        8, 9, 10, 8, 10, 11, // Left
        12, 13, 14, 12, 14, 15, // Right
        16, 17, 18, 16, 18, 19, // Top
        20, 21, 22, 20, 22, 23  // Bottom
    };

    bool Demo02_Scene::init()
	{
        PekanRenderer::enableMultisampleAntiAliasing();
        PekanRenderer::enableFaceCulling();

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
            {{  0.5f, -0.5f, -0.5f}, m_colors[0]},
            {{ -0.5f, -0.5f, -0.5f}, m_colors[0]},
            {{ -0.5f,  0.5f, -0.5f}, m_colors[0]},
            {{  0.5f,  0.5f, -0.5f}, m_colors[0]},

            // Front face (Green)
            {{-0.5f, -0.5f,  0.5f}, m_colors[1]},
            {{ 0.5f, -0.5f,  0.5f}, m_colors[1]},
            {{ 0.5f,  0.5f,  0.5f}, m_colors[1]},
            {{-0.5f,  0.5f,  0.5f}, m_colors[1]},

            // Left face (Blue)
            {{-0.5f, -0.5f, -0.5f}, m_colors[2]},
            {{-0.5f, -0.5f,  0.5f}, m_colors[2]},
            {{-0.5f,  0.5f,  0.5f}, m_colors[2]},
            {{-0.5f,  0.5f, -0.5f}, m_colors[2]},

            // Right face (Yellow)
            {{ 0.5f, -0.5f,  0.5f}, m_colors[3]},
            {{ 0.5f, -0.5f, -0.5f}, m_colors[3]},
            {{ 0.5f,  0.5f, -0.5f}, m_colors[3]},
            {{ 0.5f,  0.5f,  0.5f}, m_colors[3]},

            // Top face (Magenta)
            {{-0.5f,  0.5f,  0.5f}, m_colors[4]},
            {{ 0.5f,  0.5f,  0.5f}, m_colors[4]},
            {{ 0.5f,  0.5f, -0.5f}, m_colors[4]},
            {{-0.5f,  0.5f, -0.5f}, m_colors[4]},

            // Bottom face (Cyan)
            {{-0.5f, -0.5f, -0.5f}, m_colors[5]},
            {{ 0.5f, -0.5f, -0.5f}, m_colors[5]},
            {{ 0.5f, -0.5f,  0.5f}, m_colors[5]},
            {{-0.5f, -0.5f,  0.5f}, m_colors[5]}
        };

        PekanRenderer::enableDepthTest();

        m_renderObject.create
        (
            m_vertices.data(),
            m_vertices.size() * sizeof(Vertex),
            { { ShaderDataType::Float3, "position" }, { ShaderDataType::Float3, "color" } },
            BufferDataUsage::DynamicDraw,
            Pekan::Utils::readFileToString(vertexShaderFilePath).c_str(),
            Pekan::Utils::readFileToString(fragmentShaderFilePath).c_str()
        );
        m_renderObject.setIndexData(indices, sizeof(indices), BufferDataUsage::StaticDraw);

        // Set default transform matrices
        m_modelMatrix = glm::rotate(glm::mat4(1.0f), m_rotation * PI / 180.0f, glm::vec3(0.5f, 1.0f, 0.0f));
        m_viewMatrix = glm::lookAt(glm::vec3(m_cameraDist, m_cameraDist, m_cameraDist), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        m_projMatrix = glm::perspective(glm::radians(m_fov), float(m_resolution.x) / float(m_resolution.y), 0.01f, 100.0f);

        m_rotation = 0.0f;

        m_hideFourthFace = false;
        m_hideFourthFaceCache = false;

        return true;
	}

	void Demo02_Scene::update(double dt)
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

        m_renderObject.setVertexData
        (
            m_vertices.data(),
            m_vertices.size() * sizeof(Vertex)
        );

        if (m_hideFourthFace != m_hideFourthFaceCache)
        {
            m_hideFourthFaceCache = m_hideFourthFace;

            if (m_hideFourthFace)
            {
                const unsigned indicesReversed[] = { 14, 13, 12, 12, 15, 14 };
                m_renderObject.setIndexSubData(indicesReversed, 3 * 6 * sizeof(unsigned), 6 * sizeof(unsigned));
            }
            else
            {
                m_renderObject.setIndexSubData(&indices[18], 3 * 6 * sizeof(unsigned), 6 * sizeof(unsigned));
            }
        }

        m_rotation += float(3.0 * dt);
	}

	void Demo02_Scene::render()
	{
        PekanRenderer::clear(true);

        m_renderObject.bind();

        // Set uniform of model-view-projection matrix
        glm::mat4 mvpMatrix = m_projMatrix * m_viewMatrix * m_modelMatrix;
        m_renderObject.getShader().setUniformMatrix4fv("u_MVP", mvpMatrix);

        // Draw cube
        PekanRenderer::drawIndexed(36);

        m_renderObject.unbind();
	}

	void Demo02_Scene::exit()
	{
        m_renderObject.destroy();
	}

} // namespace Demo