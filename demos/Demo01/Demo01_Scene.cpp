#include "Demo01_Scene.h"
#include "PekanLogger.h"
#include "Utils/FileUtils.h"
#include "PekanEngine.h"
#include "RenderCommands.h"

using Pekan::PekanEngine;

#define PI 3.14159265359f

static const char* vertexShaderFilePath = "resources/01_vertex_shader.glsl";
static const char* fragmentShaderFilePath = "resources/01_fragment_shader.glsl";

using namespace Pekan::Graphics;

namespace Demo
{

    bool Demo01_Scene::init()
	{
        RenderState::enableMultisampleAntiAliasing();

        m_resolution = PekanEngine::getWindowSize();

        // Enable and configure blending
        RenderState::enableBlending();
        RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

        m_renderObject.create
        (
            { { ShaderDataType::Float2, "position" }, { ShaderDataType::Float4, "color" } },
            Pekan::FileUtils::readFileToString(vertexShaderFilePath).c_str(),
            Pekan::FileUtils::readFileToString(fragmentShaderFilePath).c_str()
        );

        t = 0.0f;

        return true;
	}

	void Demo01_Scene::update(double dt)
	{
        m_vertices.clear();
        for (const Rectangle& square : m_squares)
        {
            const float x = float(square.x);
            const float y = float(square.y);
            const float w = float(square.width);
            const float h = float(square.height);
            const float angle = float(square.rotation) * PI / 180.0f;
            const float radius = std::sqrtf((w / 2.0f) * (w / 2.0f) + (h / 2.0f) * (h / 2.0f));

            const float squareVertices[] =
            {
                // bottom left
                (x + std::cos(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / float(m_resolution.x) - 1.0f,
                (y + std::sin(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / float(m_resolution.y) - 1.0f,
                square.color.x, square.color.y, square.color.z, square.color.w,
                // bottom right
                (x + std::cos(angle - PI / 4.0f) * radius) * 2.0f / float(m_resolution.x) - 1.0f,
                (y + std::sin(angle - PI / 4.0f) * radius) * 2.0f / float(m_resolution.y) - 1.0f,
                square.color.x, square.color.y, square.color.z, square.color.w,
                // top right
                (x + std::cos(angle + PI / 4.0f) * radius) * 2.0f / float(m_resolution.x) - 1.0f,
                (y + std::sin(angle + PI / 4.0f) * radius) * 2.0f / float(m_resolution.y) - 1.0f,
                square.color.x, square.color.y, square.color.z, square.color.w,
                // bottom left
                (x + std::cos(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / float(m_resolution.x) - 1.0f,
                (y + std::sin(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / float(m_resolution.y) - 1.0f,
                square.color.x, square.color.y, square.color.z, square.color.w,
                // top right
                (x + std::cos(angle + PI / 4.0f) * radius) * 2.0f / float(m_resolution.x) - 1.0f,
                (y + std::sin(angle + PI / 4.0f) * radius) * 2.0f / float(m_resolution.y) - 1.0f,
                square.color.x, square.color.y, square.color.z, square.color.w,
                // top left
                (x + std::cos(angle + PI * 3.0f / 4.0f) * radius) * 2.0f / float(m_resolution.x) - 1.0f,
                (y + std::sin(angle + PI * 3.0f / 4.0f) * radius) * 2.0f / float(m_resolution.y) - 1.0f,
                square.color.x, square.color.y, square.color.z, square.color.w,
            };

            m_vertices.insert(m_vertices.end(), std::begin(squareVertices), std::end(squareVertices));
        }

        m_renderObject.setVertexData(m_vertices.data(), m_vertices.size() * sizeof(float), BufferDataUsage::DynamicDraw);

        // If we should move third square and there is a third square,
        // we will move it slightly just to test m_renderObject.setVertexSubData(...)
        if (m_guiWindow != nullptr && m_guiWindow->getMoveThirdSquare() && m_squares.size() >= 3)
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
            m_renderObject.setVertexSubData(&m_vertices[2 * 6 * 6], 2 * 6 * 6 * sizeof(float), 6 * 6 * sizeof(float));
        }

        t += float(dt);
	}

	void Demo01_Scene::render()
	{
        RenderCommands::clear();

        m_renderObject.bind();
        RenderCommands::draw(m_vertices.size() / 6);
        m_renderObject.unbind();
	}

	void Demo01_Scene::exit()
	{
        m_renderObject.destroy();
	}

    void Demo01_Scene::addSquare()
    {
        Rectangle newSquare;
        newSquare.width = 60;
        newSquare.height = 60;
        newSquare.x = m_resolution.x / 2;
        newSquare.y = m_resolution.y / 2;
        newSquare.id = m_squares.size();
        m_squares.push_back(newSquare);
    }

} // namespace Demo