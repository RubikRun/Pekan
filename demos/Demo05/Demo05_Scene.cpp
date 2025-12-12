#include "Demo05_Scene.h"
#include "PekanLogger.h"
#include "Utils/RandomizationUtils.h"
#include "RenderCommands.h"
#include "Renderer2DSubsystem.h"
#include "PekanTools.h"

#include <glm/gtc/constants.hpp>
constexpr float PI = glm::pi<float>();

// Parameters for Mandelbrot Times Table
constexpr glm::vec2 MTT0_CENTER = glm::vec2(-0.5f, 0.0f);
constexpr glm::vec2 MTT1_CENTER = glm::vec2(0.5f, 0.0f);
constexpr float MTT0_RADIUS = 0.45f;
constexpr float MTT1_RADIUS = 0.45f;
constexpr float MTT0_SPEED = 0.01f;
constexpr float MTT1_SPEED = 0.01f;

constexpr float CAMERA_SCALE = MTT0_RADIUS * 5.0f;

using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;
using namespace Pekan::Tools;

namespace Demo
{

    bool Demo05_Scene::init()
    {
        RenderState::enableMultisampleAntiAliasing();
        // Enable and configure blending
        RenderState::enableBlending();
        RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

        for (int i = 0; i < MTT0_SUBDIVS; i++)
        {
            m_lines[i].create({ 0.0f, 0.0f }, { 0.0f, 0.0f });
            m_lines[i].setColor({ 0.9f, 0.9f, 0.9f, 1.0f });
        }
        for (int i = 0; i < MTT1_SUBDIVS; i++)
        {
            m_lineShapes[i].create({ 0.0f, 0.0f }, { 0.0f, 0.0f });
            m_lineShapes[i].setColor({ 0.9f, 0.9f, 0.9f, 1.0f });
            m_lineShapes[i].setThickness(0.001f);
        }

        // Create camera
        m_camera = std::make_shared<Camera2D>();
        m_camera->create(CAMERA_SCALE);
        Renderer2DSubsystem::setCamera(m_camera);
        PekanTools::enableCameraController2D(m_camera);

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
        updateMtt();

        t += float(dt);
        m_mtt0factor += float(dt) * MTT0_SPEED;
        m_mtt1factor += float(dt) * MTT1_SPEED;
    }

    void Demo05_Scene::render() const
    {
        Renderer2DSubsystem::beginFrame();

        // Clear background color
        if (m_guiWindow != nullptr)
        {
            const glm::vec3 backgroundColor = m_guiWindow->getBackgroundColor();
            RenderState::setBackgroundColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
        }
        RenderCommands::clear();

        for (int i = 0; i < MTT0_SUBDIVS; i++)
        {
            m_lines[i].render();
        }
        for (int i = 0; i < MTT1_SUBDIVS; i++)
        {
            m_lineShapes[i].render();
        }

        Renderer2DSubsystem::endFrame();
    }

    void Demo05_Scene::exit()
    {
        for (int i = 0; i < MTT0_SUBDIVS; i++)
        {
            m_lines[i].destroy();
        }
        for (int i = 0; i < MTT1_SUBDIVS; i++)
        {
            m_lineShapes[i].destroy();
        }
    }

    static glm::vec2 getMtt0Point(float ang)
    {
        return glm::vec2
        (
            cos(ang) * MTT0_RADIUS + MTT0_CENTER.x,
            sin(ang) * MTT0_RADIUS + MTT0_CENTER.y
        );
    }

    static glm::vec2 getMtt1Point(float ang)
    {
        return glm::vec2
        (
            cos(ang) * MTT1_RADIUS + MTT1_CENTER.x,
            sin(ang) * MTT1_RADIUS + MTT1_CENTER.y
        );
    }

    void Demo05_Scene::updateMtt()
    {
        static constexpr float arc0 = 2.0f * PI / float(MTT0_SUBDIVS);
        for (int i = 0; i < MTT0_SUBDIVS; i++)
        {
            const float angA = float(i) * arc0;
            const float angB = float(i) * m_mtt0factor * arc0;
            m_lines[i].setPointA(getMtt0Point(angA));
            m_lines[i].setPointB(getMtt0Point(angB));
            m_lines[i].update();
        }

        static constexpr float arc1 = 2.0f * PI / float(MTT1_SUBDIVS);
        for (int i = 0; i < MTT1_SUBDIVS; i++)
        {
            const float angA = float(i) * arc1;
            const float angB = float(i) * m_mtt1factor * arc1;
            m_lineShapes[i].setPointA(getMtt1Point(angA));
            m_lineShapes[i].setPointB(getMtt1Point(angB));

            if (m_guiWindow != nullptr)
            {
                m_lineShapes[i].setThickness(m_guiWindow->getLineThickness());
            }
        }
    }

} // namespace Demo