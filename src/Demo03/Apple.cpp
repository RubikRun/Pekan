#include "Apple.h"
#include "Utils/PekanUtils.h"
#include "Snake.h"

using Pekan::PekanEngine;
using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::ShaderDataType;
using Pekan::Renderer::BufferDataUsage;
using Pekan::Utils::getRandomInt;

static const char* vertexShaderFilePath = "resources/03_apple_vertexShader.glsl";
static const char* fragmentShaderFilePath = "resources/03_apple_fragmentShader.glsl";
static const float APPLE_SIZE = 0.03f;

namespace Demo
{

    bool Apple::create(const Snake& snake)
    {
        const int windowWidth = PekanEngine::getWindowWidth();
        const int windowHeight = PekanEngine::getWindowHeight();

        m_size = int(APPLE_SIZE * float(windowHeight));
        do
        {
            m_position =
            {
                getRandomInt(snake.getThickness(), windowWidth - snake.getThickness()),
                getRandomInt(snake.getThickness(), windowHeight - snake.getThickness())
            };
        } while (snake.checkBodyOverlapsApple(*this));

        m_vertices =
        {
            m_position.x, m_position.y,
            m_position.x, m_position.y + m_size,
            m_position.x + m_size, m_position.y + m_size,
            m_position.x + m_size, m_position.y
        };

        const int indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        m_renderObject.create
        (
            m_vertices.data(),
            8 * sizeof(int),
            BufferDataUsage::DynamicDraw,
            { { ShaderDataType::Int2, "position" } },
            indices,
            6 * sizeof(int),
            BufferDataUsage::StaticDraw,
            Pekan::Utils::readFileToString(vertexShaderFilePath).c_str(),
            Pekan::Utils::readFileToString(fragmentShaderFilePath).c_str()
        );

        m_renderObject.getShader().setUniform2fv("uResolution", glm::vec2(float(windowWidth), float(windowHeight)));

        return true;
    }

    void Apple::update()
    {
    }

    void Apple::render()
    {
        m_renderObject.bind();

        // Draw apple
        PekanRenderer::drawIndexed(6);

        m_renderObject.unbind();
    }

    void Apple::destroy()
    {
        m_renderObject.destroy();
    }

    glm::ivec4 Apple::getRectangle() const
    {
        return glm::ivec4(m_position.x, m_position.y, m_size, m_size);
    }

    void Apple::regenerate(const Snake& snake)
    {
        const int windowWidth = PekanEngine::getWindowWidth();
        const int windowHeight = PekanEngine::getWindowHeight();

        do
        {
            m_position =
            {
                getRandomInt(snake.getThickness(), windowWidth - snake.getThickness()),
                getRandomInt(snake.getThickness(), windowHeight - snake.getThickness())
            };
        } while (snake.checkBodyOverlapsApple(*this));

        m_vertices =
        {
            m_position.x, m_position.y,
            m_position.x, m_position.y + m_size,
            m_position.x + m_size, m_position.y + m_size,
            m_position.x + m_size, m_position.y
        };
        m_renderObject.setVertexData(m_vertices.data(), 8 * sizeof(int), BufferDataUsage::DynamicDraw);
    }

} // namespace Demo