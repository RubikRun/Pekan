#include "Demo01_Scene.h"
#include "PekanLogger.h"
#include "Utils/FileUtils.h"
#include "PekanEngine.h"

using Pekan::PekanEngine;

#define PI 3.14159265359f

constexpr const char* const vertexShaderFilePath = "resources/01_vertex_shader.glsl";
constexpr const char* const fragmentShaderFilePath = "resources/01_fragment_shader.glsl";

using namespace Pekan::Graphics;

namespace Demo
{

    bool Demo01_Scene::init()
	{
        RenderState::enableMultisampleAntiAliasing();
        // Enable and configure blending
        RenderState::enableBlending();
        RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

        m_drawObject.create
        (
            { { ShaderDataType::Float2, "position" }, { ShaderDataType::Float4, "color" } },
            Pekan::FileUtils::readTextFileToString(vertexShaderFilePath).c_str(),
            Pekan::FileUtils::readTextFileToString(fragmentShaderFilePath).c_str()
        );

        t = 0.0f;

        return true;
	}

	void Demo01_Scene::update(double dt)
	{
        if (m_guiWindow == nullptr)
        {
            return;
        }

        // Clear vertices list
        m_vertices.clear();
        // Generate vertices of each square and fill them to vertices list
        for (size_t i = 0; i < m_guiWindow->getNumberOfSquares(); i++)
        {
            generateSquareVertices(i);
        }
        // Set draw object's vertex data to the vertices list
        m_drawObject.setVertexData(m_vertices.data(), m_vertices.size() * sizeof(float), BufferDataUsage::DynamicDraw);

        // If we should move third square and there is a third square,
        // we will move it slightly just to test m_drawObject.setVertexSubData(...)
        if (m_guiWindow->getMoveThirdSquare() && m_guiWindow->getNumberOfSquares() >= 3)
        {
            moveThirdSquare();
        }

        t += float(dt);
	}

	void Demo01_Scene::render() const
	{
        RenderCommands::clear();

        m_drawObject.render();
	}

	void Demo01_Scene::exit()
	{
        m_drawObject.destroy();
	}

    void Demo01_Scene::generateSquareVertices(size_t idx)
    {
        static const glm::vec2 windowSize = glm::vec2(PekanEngine::getWindow().getSize());

        // Get square's parameters from GUI
        const float x = float(m_guiWindow->getSquareX(idx));
        const float y = float(m_guiWindow->getSquareY(idx));
        const float w = float(m_guiWindow->getSquareSize(idx));
        const float h = float(m_guiWindow->getSquareSize(idx));
        const float angle = float(m_guiWindow->getSquareRotation(idx)) * PI / 180.0f;
        const glm::vec4 squareColor = m_guiWindow->getSquareColor(idx);
        // Calculate square's vertices based on square's parameters
        const float radius = std::sqrtf((w / 2.0f) * (w / 2.0f) + (h / 2.0f) * (h / 2.0f));
        const float squareVertices[] =
        {
            // bottom left
            (x + std::cos(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / windowSize.x - 1.0f,
            (y + std::sin(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / windowSize.y - 1.0f,
            squareColor.x, squareColor.y, squareColor.z, squareColor.w,
            // bottom right
            (x + std::cos(angle - PI / 4.0f) * radius) * 2.0f / windowSize.x - 1.0f,
            (y + std::sin(angle - PI / 4.0f) * radius) * 2.0f / windowSize.y - 1.0f,
            squareColor.x, squareColor.y, squareColor.z, squareColor.w,
            // top right
            (x + std::cos(angle + PI / 4.0f) * radius) * 2.0f / windowSize.x - 1.0f,
            (y + std::sin(angle + PI / 4.0f) * radius) * 2.0f / windowSize.y - 1.0f,
            squareColor.x, squareColor.y, squareColor.z, squareColor.w,
            // bottom left
            (x + std::cos(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / windowSize.x - 1.0f,
            (y + std::sin(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / windowSize.y - 1.0f,
            squareColor.x, squareColor.y, squareColor.z, squareColor.w,
            // top right
            (x + std::cos(angle + PI / 4.0f) * radius) * 2.0f / windowSize.x - 1.0f,
            (y + std::sin(angle + PI / 4.0f) * radius) * 2.0f / windowSize.y - 1.0f,
            squareColor.x, squareColor.y, squareColor.z, squareColor.w,
            // top left
            (x + std::cos(angle + PI * 3.0f / 4.0f) * radius) * 2.0f / windowSize.x - 1.0f,
            (y + std::sin(angle + PI * 3.0f / 4.0f) * radius) * 2.0f / windowSize.y - 1.0f,
            squareColor.x, squareColor.y, squareColor.z, squareColor.w,
        };
        // Fill square's vertices to the vertices list
        m_vertices.insert(m_vertices.end(), std::begin(squareVertices), std::end(squareVertices));
    }

    void Demo01_Scene::moveThirdSquare()
    {
        // Traverse vertex attributes of third square
        for (int i = 2 * 6 * 6; i < 3 * 6 * 6; i++)
        {
            // If vertex attribute is position
            if (i % 6 == 0 || i % 6 == 1)
            {
                // Move vertex slightly, in X and Y direction equally
                m_vertices[i] += cos(t * 15.0f) / 300.0f;
            }
        }

        // Update the region of vertex buffer the contains the third square
        m_drawObject.setVertexSubData(&m_vertices[2 * 6 * 6], 2 * 6 * 6 * sizeof(float), 6 * 6 * sizeof(float));
    }

} // namespace Demo