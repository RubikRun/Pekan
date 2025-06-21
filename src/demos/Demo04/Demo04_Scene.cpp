#include "Demo04_Scene.h"
#include "Logger/PekanLogger.h"
#include "Utils/PekanUtils.h"

#include <glm/gtc/constants.hpp>
static const float PI = glm::pi<float>();

using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::ShaderDataType;
using Pekan::Renderer::DrawMode;
using Pekan::Renderer::BufferDataUsage;
using Pekan::Renderer::Shader;
using Pekan::Renderer::BlendFactor;

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

static const int POLYGON_VERTICES_COUNT = 7;
static const float POLYGON_RADIUS = 0.2f;

namespace Demo
{

    bool Demo04_Scene::init()
    {
        PekanRenderer::enableMultisampleAntiAliasing();
        // Enable and configure blending
        PekanRenderer::enableBlending();
        PekanRenderer::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

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
            Pekan::Utils::readFileToString(VERTEX_SHADER_FILEPATHS[m_shaderIdx]).c_str(),
            Pekan::Utils::readFileToString(FRAGMENT_SHADER_FILEPATHS[m_shaderIdx]).c_str()
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

        m_polygonInitialPosition = { 0.7f, -0.7f };
        m_polygon.create({ glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f) });
        PK_ASSERT_QUICK(m_polygon.getVertices().size() == POLYGON_VERTICES_COUNT);
        m_polygon.setPosition(m_polygonInitialPosition);

        if (m_guiWindow != nullptr)
        {
            m_enabledFaceCulling = m_guiWindow->getEnabledFaceCulling();
            if (m_enabledFaceCulling)
            {
                PekanRenderer::enableFaceCulling();
            }
            else
            {
                PekanRenderer::disableFaceCulling();
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
                    Pekan::Utils::readFileToString(VERTEX_SHADER_FILEPATHS[m_shaderIdx]).c_str(),
                    Pekan::Utils::readFileToString(FRAGMENT_SHADER_FILEPATHS[m_shaderIdx]).c_str()
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
                    PekanRenderer::enableFaceCulling();
                }
                else
                {
                    PekanRenderer::disableFaceCulling();
                }
            }

            // Get position from GUI
            const ImVec2& position = m_guiWindow->getPosition();
            // Set "uPosition" uniform inside of the shader
            texRectShader.setUniform2fv("uPosition", glm::vec2(position.x, position.y));
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

        updatePolygon();

        t += float(dt) * 5.0f;
    }

    void Demo04_Scene::render()
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

        m_renderObject.bind();
        PekanRenderer::drawIndexed(6);
        m_renderObject.unbind();

        if (m_guiWindow != nullptr && m_guiWindow->isEnabledShapes())
        {
            m_triangle.render();
            m_rectangle.render();
            m_circle.render();
            m_circleStatic.render();
            m_polygon.render();
        }
    }

    void Demo04_Scene::exit()
    {
        m_renderObject.destroy();
        m_triangle.destroy();
        m_rectangle.destroy();
        m_circle.destroy();
        m_circleStatic.destroy();
        m_polygon.destroy();
    }

    void Demo04_Scene::updatePolygon()
    {
        const bool reverseOrientation = (m_guiWindow != nullptr && m_guiWindow->getReversePolygonOrientation());
        const float reverseFactor = reverseOrientation ? -1.0f : 1.0f;
        std::vector<glm::vec2> vertices(7, glm::vec2(0.0f, 0.0f));
        const float baseArc = reverseFactor * 2.0f * PI / float(POLYGON_VERTICES_COUNT);
        for (int i = 0; i < POLYGON_VERTICES_COUNT; i++)
        {
            const int iRev = reverseOrientation ? (POLYGON_VERTICES_COUNT - i) % POLYGON_VERTICES_COUNT : i;
            const float arc = baseArc * float(i);
            vertices[i] = glm::vec2
            (
                cos(arc) * POLYGON_RADIUS + osc(t / float(iRev + 1), 0.0f, float(iRev + 1) / 190.0f),
                sin(arc) * POLYGON_RADIUS + osc(t / float(POLYGON_VERTICES_COUNT - iRev), 0.0f, float(POLYGON_VERTICES_COUNT - iRev) / 190.0f)
            );
        }
        m_polygon.setVertices(vertices);

        m_polygon.setPosition(m_polygonInitialPosition + glm::vec2(sin(t / 8.0f) * 0.07f, sin(t / 6.0f) * 0.04f));
        m_polygon.setColor({ osc(t / 2.0f), osc(t / 5.0f + 2.0f), osc(t / 11.0f), osc(t / 15.0f, 0.6f, 1.0f) });
    }

} // namespace Demo