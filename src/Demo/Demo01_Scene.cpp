#include "Demo01_Scene.h"
#define PK_FILENAME "DemoScene.cpp"
#include "Logger/PekanLogger.h"
#include "Utils/PekanUtils.h"

using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::VertexBufferElement;
using Pekan::Renderer::VertexBufferLayout;
using Pekan::Renderer::VertexBufferDataUsage;
using Pekan::Renderer::ShaderDataType;
using Pekan::Renderer::DrawMode;
using Pekan::Renderer::BlendFactor;

#define PI 3.14159265359f

static const char* vertexShaderFilePath = "resources/01_vertex_shader.glsl";
static const char* fragmentShaderFilePath = "resources/01_fragment_shader.glsl";

namespace Demo
{

	void Demo01_Scene::update()
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
                (x + std::cos(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / float(m_width) - 1.0f,
                (y + std::sin(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / float(m_height) - 1.0f,
                square.color.x, square.color.y, square.color.z, square.color.w,
                // bottom right
                (x + std::cos(angle - PI / 4.0f) * radius) * 2.0f / float(m_width) - 1.0f,
                (y + std::sin(angle - PI / 4.0f) * radius) * 2.0f / float(m_height) - 1.0f,
                square.color.x, square.color.y, square.color.z, square.color.w,
                // top right
                (x + std::cos(angle + PI / 4.0f) * radius) * 2.0f / float(m_width) - 1.0f,
                (y + std::sin(angle + PI / 4.0f) * radius) * 2.0f / float(m_height) - 1.0f,
                square.color.x, square.color.y, square.color.z, square.color.w,
                // bottom left
                (x + std::cos(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / float(m_width) - 1.0f,
                (y + std::sin(angle - PI * 3.0f / 4.0f) * radius) * 2.0f / float(m_height) - 1.0f,
                square.color.x, square.color.y, square.color.z, square.color.w,
                // top right
                (x + std::cos(angle + PI / 4.0f) * radius) * 2.0f / float(m_width) - 1.0f,
                (y + std::sin(angle + PI / 4.0f) * radius) * 2.0f / float(m_height) - 1.0f,
                square.color.x, square.color.y, square.color.z, square.color.w,
                // top left
                (x + std::cos(angle + PI * 3.0f / 4.0f) * radius) * 2.0f / float(m_width) - 1.0f,
                (y + std::sin(angle + PI * 3.0f / 4.0f) * radius) * 2.0f / float(m_height) - 1.0f,
                square.color.x, square.color.y, square.color.z, square.color.w,
            };

            m_vertices.insert(m_vertices.end(), std::begin(squareVertices), std::end(squareVertices));
        }

        m_vertexArray.bind();
        m_vertexBuffer.setData(m_vertices.data(), m_vertices.size() * sizeof(float), VertexBufferDataUsage::DynamicDraw);
	}

	void Demo01_Scene::render()
	{
        // If there is no GUI window attached, just clear background with default color
        PekanRenderer::clear();

        m_shader.bind();
        m_vertexArray.bind();
        PekanRenderer::draw(m_vertices.size() / 6);
        m_vertexArray.unbind();
        m_shader.unbind();
	}

	void Demo01_Scene::exit()
	{
        m_vertexBuffer.destroy();
        m_vertexArray.destroy();
        m_shader.destroy();
	}

    void Demo01_Scene::addSquare()
    {
        Rectangle newSquare;
        newSquare.width = 60;
        newSquare.height = 60;
        newSquare.x = m_width / 2;
        newSquare.y = m_height / 2;
        newSquare.id = m_squares.size();
        m_squares.push_back(newSquare);
    }

	bool Demo01_Scene::_init()
	{
        // Enable and configure blending
        PekanRenderer::enableBlending();
        PekanRenderer::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

        m_shader.create(
            Pekan::Utils::readFileToString(vertexShaderFilePath).c_str(),
            Pekan::Utils::readFileToString(fragmentShaderFilePath).c_str()
        );

        // Create a vertex array
        m_vertexArray.create();
        // Create an empty vertex buffer, with layout specified, and add it to vertex array
        m_vertexBuffer.create();
        m_vertexArray.addVertexBuffer(m_vertexBuffer, { { ShaderDataType::Float2, "position" }, { ShaderDataType::Float4, "color" } });

        return true;
	}

} // namespace Demo