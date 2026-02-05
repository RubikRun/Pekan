#include "Apple.h"
#include "Snake.h"

#include "Utils/FileUtils.h"
#include "Utils/RandomizationUtils.h"

constexpr const char* const vertexShaderFilePath = "resources/03_apple_vertexShader.glsl";
constexpr const char* const fragmentShaderFilePath = "resources/03_apple_fragmentShader.glsl";
constexpr float APPLE_SIZE = 0.03f;

using Pekan::PekanEngine;
using Pekan::RandomizationUtils::getRandomInt;
using namespace Pekan::Graphics;

namespace Demo
{

    bool Apple::create(const Snake& snake)
    {
        m_resolution = PekanEngine::getWindow().getSize();

        m_size = int(APPLE_SIZE * float(m_resolution.y));
        do
        {
            m_position =
            {
                getRandomInt(snake.getThickness(), m_resolution.x - snake.getThickness()),
                getRandomInt(snake.getThickness(), m_resolution.y - snake.getThickness())
            };
        } while (snake.checkBodyOverlapsApple(*this));

        m_vertices =
        {
            m_position.x, m_position.y,
            m_position.x, m_position.y + m_size,
            m_position.x + m_size, m_position.y + m_size,
            m_position.x + m_size, m_position.y
        };

        constexpr int indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        m_drawObject.create
        (
            m_vertices.data(),
            8 * sizeof(int),
            { { ShaderDataType::Int2, "position" } },
            BufferDataUsage::DynamicDraw,
            Pekan::FileUtils::readTextFileToString(vertexShaderFilePath).c_str(),
            Pekan::FileUtils::readTextFileToString(fragmentShaderFilePath).c_str()
        );
        m_drawObject.setIndexData(indices, 6 * sizeof(int), BufferDataUsage::StaticDraw);

        m_drawObject.getShader().setUniform2f("uResolution", glm::vec2(float(m_resolution.x), float(m_resolution.y)));

        return true;
    }

    void Apple::update()
    {
    }

    void Apple::render() const
    {
        m_drawObject.render();
    }

    void Apple::destroy()
    {
        m_drawObject.destroy();
    }

    glm::ivec4 Apple::getRectangle() const
    {
        return glm::ivec4(m_position.x, m_position.y, m_size, m_size);
    }

    void Apple::regenerate(const Snake& snake)
    {
        do
        {
            m_position =
            {
                getRandomInt(snake.getThickness(), m_resolution.x - snake.getThickness()),
                getRandomInt(snake.getThickness(), m_resolution.y - snake.getThickness())
            };
        } while (snake.checkBodyOverlapsApple(*this));

        m_vertices =
        {
            m_position.x, m_position.y,
            m_position.x, m_position.y + m_size,
            m_position.x + m_size, m_position.y + m_size,
            m_position.x + m_size, m_position.y
        };
        m_drawObject.setVertexData(m_vertices.data(), 8 * sizeof(int));
    }

} // namespace Demo