#include "Apple.h"
#include "Utils/PekanUtils.h"

using Pekan::PekanEngine;
using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::ShaderDataType;
using Pekan::Renderer::BufferDataUsage;
using Pekan::Utils::getRandomFloat;

static const char* vertexShaderFilePath = "resources/03_apple_vertexShader.glsl";
static const char* fragmentShaderFilePath = "resources/03_apple_fragmentShader.glsl";
static const float APPLE_SIZE = 0.03f;

namespace Demo
{

    bool Apple::create()
    {
        m_aspectRatio = float(PekanEngine::getWindowWidth()) / float(PekanEngine::getWindowHeight());

        m_position =
        {
            getRandomFloat(-1.0f, 1.0f - APPLE_SIZE),
            getRandomFloat(-1.0f, 1.0f - APPLE_SIZE)
        };

        m_vertices =
        {
            m_position.x, m_position.y,
            m_position.x, m_position.y + APPLE_SIZE * m_aspectRatio,
            m_position.x + APPLE_SIZE, m_position.y + APPLE_SIZE * m_aspectRatio,
            m_position.x + APPLE_SIZE, m_position.y
        };

        const int indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        m_renderObject.create
        (
            m_vertices.data(),
            8 * sizeof(float),
            BufferDataUsage::DynamicDraw,
            { { ShaderDataType::Float2, "position" } },
            indices,
            6 * sizeof(int),
            BufferDataUsage::StaticDraw,
            Pekan::Utils::readFileToString(vertexShaderFilePath).c_str(),
            Pekan::Utils::readFileToString(fragmentShaderFilePath).c_str()
        );

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

} // namespace Demo