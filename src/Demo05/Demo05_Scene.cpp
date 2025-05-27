#include "Demo05_Scene.h"
#include "Logger/PekanLogger.h"
#include "Utils/PekanUtils.h"

using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::ShaderDataType;
using Pekan::Renderer::DrawMode;
using Pekan::Renderer::BufferDataUsage;
using Pekan::Renderer::Shader;
using Pekan::Renderer::BlendFactor;

namespace Demo
{

    bool Demo05_Scene::init()
    {
        PekanRenderer::enableMultisampleAntiAliasing();
        // Enable and configure blending
        PekanRenderer::enableBlending();
        PekanRenderer::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

        m_line.create({ -0.5f, -0.5f }, { 0.5f, 0.5f });
        m_line.setColor({ 0.9f, 0.9f, 0.9f, 1.0f });

        m_lineShape.create({ -0.5f, 0.5f }, { 0.5f, -0.5f });
        m_lineShape.setColor({ 0.9f, 0.9f, 0.9f, 1.0f });

        t = 0.0f;

        return true;
    }

    static float osc(float x)
    {
        return cos(x) * 0.5f + 0.5f;
    }
    static float osc(float x, float a, float b)
    {
        return a + (b - a) * osc(x);
    }

    void Demo05_Scene::update(double dt)
    {
        const float THICKNESS_AMPL = 0.2f;

        m_line.setColor({ osc(t / 25.0f + 3.0f), osc(t / 15.0f), osc(t / 35.0f), osc(t / 35.0f, 0.3f, 1.0f) });

        m_lineShape.setThickness(THICKNESS_AMPL + cos(t) * THICKNESS_AMPL);
        m_lineShape.setColor({ osc(t / 10.0f), osc(t / 20.0f + 2.0f), osc(t / 30.0f), osc(t / 3.0f, 0.6f, 1.0f) });

        t += float(dt) * 3.0f;
    }

    void Demo05_Scene::render()
    {
        // Clear background color
        if (m_guiWindow != nullptr)
        {
            const ImVec4& clearColor = m_guiWindow->getClearColor();
            PekanRenderer::setBackgroundColor(glm::vec4(clearColor.x, clearColor.y, clearColor.z, clearColor.w));
            PekanRenderer::clear();
        }
        else
        {
            // If there is no GUI window attached, just clear background with default color
            PekanRenderer::clear();
        }

        m_line.render();
        m_lineShape.render();
    }

    void Demo05_Scene::exit()
    {
        m_line.destroy();
        m_lineShape.destroy();
    }

} // namespace Demo