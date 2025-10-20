#include "Demo02_Scene.h"
#include "Utils/FileUtils.h"
#include "PekanLogger.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using Pekan::Graphics::RenderCommands;
using Pekan::Graphics::RenderState;
using Pekan::Graphics::ShaderDataType;
using Pekan::Graphics::BufferDataUsage;
using Pekan::PekanEngine;

#define PI 3.14159265359f

constexpr char* const vertexShaderFilePath = "resources/02_vertex_shader.glsl";
constexpr char* const fragmentShaderFilePath = "resources/02_fragment_shader.glsl";

constexpr float COLOR_DULLNESS = 0.9f;

namespace Demo
{

    constexpr unsigned indices[] =
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
        RenderState::enableMultisampleAntiAliasing();
        RenderState::enableFaceCulling();

        if (m_guiWindow == nullptr)
        {
            PK_LOG_ERROR("GUI window is not attached to Demo02_Scene. Cannot initialize scene.", "Pekan");
            return false;
        }

        static const glm::ivec2 windowSize = PekanEngine::getWindow().getSize();

        // Set default colors of cube's sides
        const glm::vec3 colors[6] =
        {
            m_guiWindow->getFaceColor(0), m_guiWindow->getFaceColor(1),
            m_guiWindow->getFaceColor(2), m_guiWindow->getFaceColor(3),
            m_guiWindow->getFaceColor(4), m_guiWindow->getFaceColor(5)
        };

        // Initialize vertices of cube
        m_vertices =
        {
            // Back face (Red)
            {{  0.5f, -0.5f, -0.5f}, colors[0]},
            {{ -0.5f, -0.5f, -0.5f}, colors[0]},
            {{ -0.5f,  0.5f, -0.5f}, colors[0]},
            {{  0.5f,  0.5f, -0.5f}, colors[0]},

            // Front face (Green)
            {{-0.5f, -0.5f,  0.5f}, colors[1]},
            {{ 0.5f, -0.5f,  0.5f}, colors[1]},
            {{ 0.5f,  0.5f,  0.5f}, colors[1]},
            {{-0.5f,  0.5f,  0.5f}, colors[1]},

            // Left face (Blue)
            {{-0.5f, -0.5f, -0.5f}, colors[2]},
            {{-0.5f, -0.5f,  0.5f}, colors[2]},
            {{-0.5f,  0.5f,  0.5f}, colors[2]},
            {{-0.5f,  0.5f, -0.5f}, colors[2]},

            // Right face (Yellow)
            {{ 0.5f, -0.5f,  0.5f}, colors[3]},
            {{ 0.5f, -0.5f, -0.5f}, colors[3]},
            {{ 0.5f,  0.5f, -0.5f}, colors[3]},
            {{ 0.5f,  0.5f,  0.5f}, colors[3]},

            // Top face (Magenta)
            {{-0.5f,  0.5f,  0.5f}, colors[4]},
            {{ 0.5f,  0.5f,  0.5f}, colors[4]},
            {{ 0.5f,  0.5f, -0.5f}, colors[4]},
            {{-0.5f,  0.5f, -0.5f}, colors[4]},

            // Bottom face (Cyan)
            {{-0.5f, -0.5f, -0.5f}, colors[5]},
            {{ 0.5f, -0.5f, -0.5f}, colors[5]},
            {{ 0.5f, -0.5f,  0.5f}, colors[5]},
            {{-0.5f, -0.5f,  0.5f}, colors[5]}
        };

        RenderState::enableDepthTest();

        m_renderObject.create
        (
            m_vertices.data(),
            m_vertices.size() * sizeof(Vertex),
            { { ShaderDataType::Float3, "position" }, { ShaderDataType::Float3, "color" } },
            BufferDataUsage::DynamicDraw,
            Pekan::FileUtils::readTextFileToString(vertexShaderFilePath).c_str(),
            Pekan::FileUtils::readTextFileToString(fragmentShaderFilePath).c_str()
        );
        m_renderObject.setIndexData(indices, sizeof(indices), BufferDataUsage::StaticDraw);

        // Get parameters from GUI window
        const float rotation = m_guiWindow->getRotation();
        const float cameraDist = m_guiWindow->getCameraDist();
        const float fov = m_guiWindow->getFOV();

        // Set default transform matrices
        m_modelMatrix = glm::rotate(glm::mat4(1.0f), rotation * PI / 180.0f, glm::vec3(0.5f, 1.0f, 0.0f));
        m_viewMatrix = glm::lookAt(glm::vec3(cameraDist, cameraDist, cameraDist), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        m_projMatrix = glm::perspective(glm::radians(fov), float(windowSize.x) / float(windowSize.y), 0.01f, 100.0f);

        m_hideFourthFaceCache = false;

        return true;
	}

	void Demo02_Scene::update(double dt)
	{
        if (m_guiWindow == nullptr)
        {
            PK_LOG_ERROR("GUI window is not attached to Demo02_Scene. Cannot update scene.", "Pekan");
            return;
        }

        static const glm::ivec2 windowSize = PekanEngine::getWindow().getSize();

        // Get current parameters from GUI window
        const float rotation = m_guiWindow->getRotation();
        const float cameraDist = m_guiWindow->getCameraDist();
        const float fov = m_guiWindow->getFOV();
        // Set model, view, and projection matrices with current parameters
        m_modelMatrix = glm::rotate(glm::mat4(1.0f), rotation * PI / 180.0f, glm::vec3(0.5f, 1.0f, 0.0f));
        m_viewMatrix = glm::lookAt(glm::vec3(cameraDist, cameraDist, cameraDist), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        m_projMatrix = glm::perspective(glm::radians(fov), float(windowSize.x) / float(windowSize.y), 0.01f, 100.0f);
        // Set model-view-projection matrix uniform inside the shader
        glm::mat4 mvpMatrix = m_projMatrix * m_viewMatrix * m_modelMatrix;
        m_renderObject.getShader().setUniformMatrix4fv("u_MVP", mvpMatrix);

        for (size_t i = 0; i < 6; i++)
        {
            const glm::vec3 color = m_guiWindow->getFaceColor(i);
            m_vertices[i * 4 + 0].color = color;
            m_vertices[i * 4 + 1].color = color;
            m_vertices[i * 4 + 2].color = color;
            m_vertices[i * 4 + 3].color = color;
        }

        m_renderObject.setVertexData
        (
            m_vertices.data(),
            m_vertices.size() * sizeof(Vertex)
        );

        if (m_guiWindow->shouldHideFourthFace() != m_hideFourthFaceCache)
        {
            m_hideFourthFaceCache = !m_hideFourthFaceCache;

            if (m_hideFourthFaceCache)
            {
                static constexpr unsigned indicesReversed[] = { 14, 13, 12, 12, 15, 14 };
                m_renderObject.setIndexSubData(indicesReversed, 3 * 6 * sizeof(unsigned), 6 * sizeof(unsigned));
            }
            else
            {
                m_renderObject.setIndexSubData(&indices[18], 3 * 6 * sizeof(unsigned), 6 * sizeof(unsigned));
            }
        }
	}

	void Demo02_Scene::render() const
	{
        RenderCommands::clear(true, true);
        // Render cube
        m_renderObject.render();
	}

	void Demo02_Scene::exit()
	{
        m_renderObject.destroy();
	}

} // namespace Demo