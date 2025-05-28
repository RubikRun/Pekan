#include "Demo01_Scene.h"
#include "Logger/PekanLogger.h"
#include "Utils/PekanUtils.h"
#include "PekanEngine.h"

using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::BufferDataUsage;
using Pekan::Renderer::ShaderDataType;
using Pekan::Renderer::DrawMode;
using Pekan::Renderer::BlendFactor;
using Pekan::PekanEngine;

#define PI 3.14159265359f

static const char* vertexShaderFilePath = "resources/01_vertex_shader.glsl";
static const char* fragmentShaderFilePath = "resources/01_fragment_shader.glsl";

namespace Demo
{

    bool Demo01_Scene::init()
	{
        PekanRenderer::enableMultisampleAntiAliasing();

        m_resolution = PekanEngine::getWindowResolution();

        // Enable and configure blending
        PekanRenderer::enableBlending();
        PekanRenderer::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

        m_renderObject.create
        (
            { { ShaderDataType::Float2, "position" }, { ShaderDataType::Float4, "color" } },
            Pekan::Utils::readFileToString(vertexShaderFilePath).c_str(),
            Pekan::Utils::readFileToString(fragmentShaderFilePath).c_str()
        );

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
	}

	void Demo01_Scene::render()
	{
        PekanRenderer::clear();

        m_renderObject.bind();
        PekanRenderer::draw(m_vertices.size() / 6);
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