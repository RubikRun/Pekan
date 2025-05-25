#include "Demo05_Scene.h"
#include "Logger/PekanLogger.h"
#include "Utils/PekanUtils.h"

using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::ShaderDataType;
using Pekan::Renderer::DrawMode;
using Pekan::Renderer::BufferDataUsage;
using Pekan::Renderer::Shader;

namespace Demo
{

    bool Demo05_Scene::init()
    {
        PekanRenderer::enableMultisampleAntiAliasing();

        m_line.create({ -0.5f, -0.5f }, { 0.5f, 0.5f });
        m_line.setColor({ 0.9f, 0.9f, 0.9f, 1.0f });
        return true;
    }

    void Demo05_Scene::update(double dt)
    {
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
    }

    void Demo05_Scene::exit()
    {
        m_line.destroy();
    }

} // namespace Demo