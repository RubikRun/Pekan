#include "Demo04_Scene.h"
#include "PekanLogger.h"
#include "Utils/FileUtils.h"
#include "RenderCommands.h"
#include "Renderer2DSystem.h"

#include <glm/gtc/constants.hpp>
static const float PI = glm::pi<float>();

static const char* VERTEX_SHADER_FILEPATHS[] =
{
    "resources/04_00_vertex_shader.glsl",
    "resources/04_01_vertex_shader.glsl",
    "resources/04_02_vertex_shader.glsl",
    "resources/04_03_vertex_shader.glsl"
};

static const char* FRAGMENT_SHADER_FILEPATHS[] =
{
    "resources/04_00_fragment_shader.glsl",
    "resources/04_01_fragment_shader.glsl",
    "resources/04_02_fragment_shader.glsl",
    "resources/04_03_fragment_shader.glsl"
};

static const char* IMAGE0_FILEPATH = "resources/tmnt.png";
static const char* IMAGE1_FILEPATH = "resources/powerpuff.png";

static const int POLYGON1_VERTICES_COUNT = 7;
static const float POLYGON1_RADIUS = 0.2f;
static const std::vector<glm::vec2> POLYGON2_VERTICES =
{
    { 0.20040983606557375, 0.14385245901639346 },
    { 0.1081967213114754, 0.11987704918032788 },
    { 0.03688524590163935, 0.1653688524590164 },
    { 0.06147540983606554, 0.11741803278688524 },
    { -0.0264344262295082, 0.1112704918032787 },
    { 0.0049180327868852515, 0.09651639344262294 },
    { -0.023360655737704926, 0.06516393442622949 },
    { -0.0540983606557377, 0.1014344262295082 },
    { -0.04795081967213115, 0.14323770491803275 },
    { 0.04057377049180331, 0.1413934426229508 },
    { 0.03995901639344259, 0.14692622950819673 },
    { -0.04795081967213115, 0.21454918032786885 },
    { -0.10573770491803276, 0.08114754098360655 },
    { -0.1875, 0.1973360655737705 },
    { -0.21577868852459015, 0.003073770491803274 },
    { -0.2723360655737705, -0.07438524590163935 },
    { -0.21024590163934423, -0.16045081967213115 },
    { -0.10020491803278687, -0.1930327868852459 },
    { -0.0110655737704918, -0.0737704918032787 },
    { -0.014139344262295073, -0.07069672131147539 },
    { -0.10020491803278687, -0.15799180327868848 },
    { -0.15061475409836064, -0.1444672131147541 },
    { -0.09098360655737704, -0.027663934426229497 },
    { -0.1069672131147541, -0.12540983606557374 },
    { 0.030737704918032804, 0.05655737704918032 },
    { 0.0805327868852459, -0.08545081967213115 },
    { 0.07315573770491805, -0.0055327868852459 },
    { 0.21209016393442626, -0.094672131147541 },
    { 0.22622950819672127, 0.08790983606557377 },
    { 0.1641393442622951, 0.007377049180327877 },
    { 0.15799180327868848, 0.09713114754098359 },
    { 0.1875, 0.06823770491803277 }
};

using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;

namespace Demo
{

    bool Demo04_Scene::init()
    {
        RenderState::enableMultisampleAntiAliasing();
        // Enable and configure blending
        RenderState::enableBlending();
        RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

        // Set up vertex data and configure vertex attributes
        const float vertices[] =
        {
            -0.8f, -0.8f, 0.0f, 0.0f, // bottom left
             0.8f, -0.8f, 1.0f, 0.0f, // bottom right
             0.8f,  0.8f, 1.0f, 1.0f, // top right
            -0.8f,  0.8f, 0.0f, 1.0f  // top left
        };
        const unsigned indices[] =
        {
            0, 1, 2,
            0, 2, 3
        };

        if (m_guiWindow != nullptr)
        {
            m_shaderIdx = m_guiWindow->getShaderIdx();
        }
        else
        {
            m_shaderIdx = 0;
        }

        m_renderObject.create
        (
            vertices, sizeof(vertices),
            { { ShaderDataType::Float2, "position" }, { ShaderDataType::Float2, "texCoord" } },
            BufferDataUsage::StaticDraw,
            Pekan::FileUtils::readFileToString(VERTEX_SHADER_FILEPATHS[m_shaderIdx]).c_str(),
            Pekan::FileUtils::readFileToString(FRAGMENT_SHADER_FILEPATHS[m_shaderIdx]).c_str()
        );
        m_renderObject.setIndexData(indices, sizeof(indices), BufferDataUsage::StaticDraw);

        // Load the two images
        m_image0.load(IMAGE0_FILEPATH);
        m_image1.load(IMAGE1_FILEPATH);
        // Set two images as textures to the render object
        m_renderObject.setTextureImage(m_image0, "uTex0", 0);
        m_renderObject.setTextureImage(m_image1, "uTex1", 1);

        m_triangle.create({ 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f });

        m_rectangleInitialWidth = 0.2f;
        m_rectangleInitialHeight = 0.4f;
        m_rectangle.create(m_rectangleInitialWidth, m_rectangleInitialHeight);

        m_rectangleInitialPosition = { -0.7f, -0.7f };
        m_rectangle.setPosition(m_rectangleInitialPosition);

        m_circleInitialRadius = 0.3f;
        m_circle.create(m_circleInitialRadius);

        m_circleInitialPosition = { 0.0f, 0.0f };
        m_circle.setPosition(m_circleInitialPosition);

        m_circleStaticInitialRadius = 0.2f;
        m_circleStatic.create(m_circleStaticInitialRadius);

        m_circleStaticInitialPosition = { 0.0f, 0.0f };
        m_circleStatic.setPosition(m_circleStaticInitialPosition);

        m_polygon1InitialPosition = { 0.7f, -0.7f };
        m_polygon1.create({ {0.0f, 0.0f} });
        m_polygon1.setPosition(m_polygon1InitialPosition);

        m_polygon2InitialPosition = { -0.7f, 0.7f };
        m_polygon2.create(POLYGON2_VERTICES);
        m_polygon2.setPosition(m_polygon2InitialPosition);

        if (m_guiWindow != nullptr)
        {
            m_enabledFaceCulling = m_guiWindow->getEnabledFaceCulling();
            if (m_enabledFaceCulling)
            {
                RenderState::enableFaceCulling();
            }
            else
            {
                RenderState::disableFaceCulling();
            }
        }

        t = 0.0f;

        return true;
    }

    // Oscillates between 0 and 1 in a sine wave, as x grows
    static float osc(float x)
    {
        return (cos(x) + 1.0f) / 2.0f;
    }
    static float osc(float x, float a, float b)
    {
        return a + (b - a) * osc(x);
    }

    void Demo04_Scene::update(double dt)
    {
        Shader& texRectShader = m_renderObject.getShader();
        texRectShader.bind();

        if (m_guiWindow != nullptr)
        {
            // If selected shader in GUI has changed
            const int guiShaderIdx = m_guiWindow->getShaderIdx();
            if (m_shaderIdx != guiShaderIdx)
            {
                m_shaderIdx = guiShaderIdx;
                // then set new shader's source code
                m_renderObject.setShaderSource
                (
                    Pekan::FileUtils::readFileToString(VERTEX_SHADER_FILEPATHS[m_shaderIdx]).c_str(),
                    Pekan::FileUtils::readFileToString(FRAGMENT_SHADER_FILEPATHS[m_shaderIdx]).c_str()
                );
                // We need to set textures again after changing the shader
                m_renderObject.setTextureImage(m_image0, "uTex0", 0);
                m_renderObject.setTextureImage(m_image1, "uTex1", 1);
            }

            // If enabled/disabled state of face culling has changed in GUI
            if (m_enabledFaceCulling != m_guiWindow->getEnabledFaceCulling())
            {
                m_enabledFaceCulling = !m_enabledFaceCulling;
                if (m_enabledFaceCulling)
                {
                    RenderState::enableFaceCulling();
                }
                else
                {
                    RenderState::disableFaceCulling();
                }
            }

            // Get position from GUI
            const glm::vec2 position = m_guiWindow->getPosition();
            // Set "uPosition" uniform inside of the shader
            texRectShader.setUniform2fv("uPosition", position);
        }

        texRectShader.setUniform1f("uTime", t);

        m_triangle.setPosition(glm::vec2(0.8f, 0.8f) + glm::vec2(sin(t) * 0.1f, sin(t / 4.0f) * 0.05f));
        m_triangle.setColor({ osc(t), osc(t / 2.0f + 2.0f), osc(t / 3.0f), osc(t / 3.0f, 0.3f, 1.0f) });

        if (m_guiWindow != nullptr && m_guiWindow->getReverseTriangleOrientation())
        {
            m_triangle.setVertexA(glm::vec2(0.1f, 0.1f) + glm::vec2(0.0f, sin(t / 5.0f) * 0.03f));
            m_triangle.setVertexB(glm::vec2(0.1f, -0.1f) + glm::vec2(cos(t * 2.0f) * sin(t) * 0.05f, sin(t * 0.83f) * 0.1f));
            m_triangle.setVertexC(glm::vec2(-0.1f, -0.1f) + glm::vec2(cos(t) * 0.1f, sin(t) * 0.1f));
        }
        else
        {
            m_triangle.setVertexA(glm::vec2(-0.1f, -0.1f) + glm::vec2(cos(t) * 0.1f, sin(t) * 0.1f));
            m_triangle.setVertexB(glm::vec2(0.1f, -0.1f) + glm::vec2(cos(t * 2.0f) * sin(t) * 0.05f, sin(t * 0.83f) * 0.1f));
            m_triangle.setVertexC(glm::vec2(0.1f, 0.1f) + glm::vec2(0.0f, sin(t / 5.0f) * 0.03f));
        }

        m_rectangle.setPosition(m_rectangleInitialPosition + glm::vec2(sin(t / 2.0f) * 0.12f, sin(t / 5.0f) * 0.04f));
        m_rectangle.setColor({ osc(t / 2.0f + 1.0f), osc(t), osc(t / 3.0f), osc(t / 7.0f, 0.3f, 1.0f) });
        m_rectangle.setWidth(osc(t / 2.0f, m_rectangleInitialWidth * 0.5f, m_rectangleInitialWidth * 1.5f));
        m_rectangle.setHeight(osc(t / 5.0f, m_rectangleInitialHeight * 0.5f, m_rectangleInitialHeight * 1.5f));

        m_circle.setSegmentsCount(int(osc(t / 5.0f, 3.0f, 30.0f)));

        m_circleStatic.setPosition(m_circleInitialPosition[1] + glm::vec2(sin(t / 3.0f) * 0.09f, sin(t / 5.0f) * 0.03f));
        m_circleStatic.setColor({ osc(t / 7.0f + 1.0f), osc(t / 2.0f + 2.0f), osc(t / 3.0f), osc(t / 3.0f, 0.3f, 1.0f) });
        m_circleStatic.setRadius(osc(t / 2.0f, m_circleStaticInitialRadius * 0.4f, m_circleStaticInitialRadius * 1.0f));

        updatePolygon1();
        updatePolygon2();

        t += float(dt) * 5.0f;
    }

    void Demo04_Scene::render() const
    {
        Renderer2DSystem::beginFrame();

        // Clear background color
        if (m_guiWindow != nullptr)
        {
            const glm::vec4 backgroundColor = m_guiWindow->getBackgroundColor();
            RenderState::setBackgroundColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
            RenderCommands::clear();
        }
        else
        {
            // If there is no GUI window attached, just clear background with default color
            RenderCommands::clear();
        }

        m_renderObject.bind();
        RenderCommands::drawIndexed(6);
        m_renderObject.unbind();

        if (m_guiWindow != nullptr && m_guiWindow->isEnabledShapes())
        {
            m_triangle.render();
            m_rectangle.render();
            m_circle.render();
            m_circleStatic.render();
            m_polygon1.render();
            m_polygon2.render();
        }

        Renderer2DSystem::endFrame();
    }

    void Demo04_Scene::exit()
    {
        m_renderObject.destroy();
        m_triangle.destroy();
        m_rectangle.destroy();
        m_circle.destroy();
        m_circleStatic.destroy();
        m_polygon1.destroy();
        m_polygon2.destroy();
    }

    void Demo04_Scene::updatePolygon1()
    {
        const bool reverseOrientation = (m_guiWindow != nullptr && m_guiWindow->getReversePolygonOrientation());
        const float reverseFactor = reverseOrientation ? -1.0f : 1.0f;
        std::vector<glm::vec2> vertices(POLYGON1_VERTICES_COUNT, glm::vec2(0.0f, 0.0f));
        const float baseArc = reverseFactor * 2.0f * PI / float(POLYGON1_VERTICES_COUNT);
        for (int i = 0; i < POLYGON1_VERTICES_COUNT; i++)
        {
            const int iRev = reverseOrientation ? (POLYGON1_VERTICES_COUNT - i) % POLYGON1_VERTICES_COUNT : i;
            const float arc = baseArc * float(i);
            vertices[i] = glm::vec2
            (
                cos(arc) * POLYGON1_RADIUS + osc(t / float(iRev + 1), 0.0f, float(iRev + 1) / 190.0f),
                sin(arc) * POLYGON1_RADIUS + osc(t / float(POLYGON1_VERTICES_COUNT - iRev), 0.0f, float(POLYGON1_VERTICES_COUNT - iRev) / 190.0f)
            );
        }
        m_polygon1.setVertices(vertices);

        m_polygon1.setPosition(m_polygon1InitialPosition + glm::vec2(sin(t / 8.0f) * 0.07f, sin(t / 6.0f) * 0.04f));
        m_polygon1.setColor({ osc(t / 2.0f), osc(t / 5.0f + 2.0f), osc(t / 11.0f), osc(t / 15.0f, 0.6f, 1.0f) });
    }

    void Demo04_Scene::updatePolygon2()
    {
        m_polygon2.setPosition(m_polygon2InitialPosition + glm::vec2(sin(t / 8.0f) * 0.02f, sin(t / 6.0f) * 0.03f));
        m_polygon2.setColor({ osc(t / 3.0f), osc(t / 2.0f + 5.0f), osc(t / 8.0f), osc(t / 11.0f, 0.6f, 1.0f) });
    }

} // namespace Demo