#include "Snake.h"
#include "Utils/PekanUtils.h"

using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::ShaderDataType;
using Pekan::Renderer::BufferDataUsage;
using Pekan::PekanEngine;

static const char* vertexShaderFilePath = "resources/03_snake_vertexShader.glsl";
static const char* fragmentShaderFilePath = "resources/03_snake_fragmentShader.glsl";
static const float THICKNESS = 0.07f;
static const float INITIAL_POSITION_X = 0.5f;
static const float INITIAL_POSITION_Y = 0.5f;
// The snake will move by 1 square every MOVE_FRAMES frames
static const int MOVE_FRAMES = 20;

namespace Demo
{
    static bool checkOverlapRectangles(const glm::vec4 r1, const glm::vec4 r2)
    {
        return (!(r2.x + r2.z < r1.x || r2.x > r1.x + r1.z || r2.y + r2.w < r1.y || r2.y > r1.y + r1.w));
    }

	bool Snake::create()
	{
        m_squaresCount = 4;
        m_headIdx = 0;
        m_tailIdx = 3;

        const int windowWidth = PekanEngine::getWindowWidth();
        const int windowHeight = PekanEngine::getWindowHeight();
        m_thickness = THICKNESS * windowHeight;
        const int posX = int(INITIAL_POSITION_X * windowWidth);
        const int posY = int(INITIAL_POSITION_Y * windowHeight);

        // Initialize vertices of snake
        m_vertices =
        {
            // square 0
            posX, posY, // top left
            posX, posY + m_thickness, // bottom left
            posX + m_thickness, posY + m_thickness, // bottom right
            posX + m_thickness, posY, // top right
            // square 1
            posX + m_thickness, posY, // top left
            posX + m_thickness, posY + m_thickness, // bottom left
            posX + 2 * m_thickness, posY + m_thickness, // bottom right
            posX + 2 * m_thickness, (posY), // top right
            // square 2
            posX + 2 * m_thickness, posY, // top left
            posX + 2 * m_thickness, posY + m_thickness, // bottom left
            posX + 3 * m_thickness, posY + m_thickness, // bottom right
            posX + 3 * m_thickness, posY, // top right
            // square 3
            posX + 3 * m_thickness, posY, // top left
            posX + 3 * m_thickness, posY + m_thickness, // bottom left
            posX + 4 * m_thickness, posY + m_thickness, // bottom right
            posX + 4 * m_thickness, posY, // top right
        };

        m_indices =
        {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            8, 9, 10, 10, 11, 8,
            12, 13, 14, 14, 15, 12
        };

        m_renderObject.create
        (
            m_vertices.data(),
            m_vertices.size() * sizeof(int),
            BufferDataUsage::DynamicDraw,
            { { ShaderDataType::Int2, "position" } },
            m_indices.data(),
            m_indices.size() * sizeof(int),
            BufferDataUsage::StaticDraw,
            Pekan::Utils::readFileToString(vertexShaderFilePath).c_str(),
            Pekan::Utils::readFileToString(fragmentShaderFilePath).c_str()
        );

        m_renderObject.getShader().setUniform2fv("uResolution", glm::vec2(float(windowWidth), float(windowHeight)));

        return true;
	}

    void Snake::update()
    {
        m_frames++;

        if (PekanEngine::isKeyPressed_W())
        {
            m_direction = { 0, -1 };
        }
        else if (PekanEngine::isKeyPressed_A())
        {
            m_direction = { -1, 0 };
        }
        else if (PekanEngine::isKeyPressed_S())
        {
            m_direction = { 0, 1 };
        }
        else if (PekanEngine::isKeyPressed_D())
        {
            m_direction = { 1, 0 };
        }

        if (m_frames % MOVE_FRAMES == 0)
        {
            move();
        }
    }

    void Snake::render()
    {
        m_renderObject.bind();

        // Draw snake
        PekanRenderer::drawIndexed(m_indices.size());

        m_renderObject.unbind();
    }

    void Snake::destroy()
    {
        m_renderObject.destroy();
    }

    bool Snake::checkHeadOverlapsApple(const Apple& apple) const
    {
        const glm::ivec4 appleRect = apple.getRectangle();
        const glm::ivec4 headRect = getRectangle(m_headIdx);
        return checkOverlapRectangles(appleRect, headRect);
    }

    bool Snake::checkBodyOverlapsApple(const Apple& apple) const
    {
        const glm::ivec4 appleRect = apple.getRectangle();
        for (int i = 0; i < m_squaresCount; i++)
        {
            const glm::ivec4 bodyRect = getRectangle(i);
            if (checkOverlapRectangles(appleRect, bodyRect))
            {
                return true;
            }
        }
        return false;
    }

    void Snake::move()
    {
        setSquarePosition(m_tailIdx, getSquarePosition(m_headIdx) + m_direction * m_thickness);
        m_headIdx = m_tailIdx;
        m_tailIdx -= 1;
        if (m_tailIdx < 0)
        {
            m_tailIdx = m_squaresCount - 1;
        }

        m_renderObject.setVertexData(m_vertices.data(), m_vertices.size() * sizeof(int), BufferDataUsage::DynamicDraw);
    }

    void Snake::setSquarePosition(int idx, glm::ivec2 pos)
    {
        m_vertices[idx * 8 + 0] = pos.x;
        m_vertices[idx * 8 + 1] = pos.y;
        m_vertices[idx * 8 + 2] = pos.x;
        m_vertices[idx * 8 + 3] = pos.y + m_thickness;
        m_vertices[idx * 8 + 4] = pos.x + m_thickness;
        m_vertices[idx * 8 + 5] = pos.y + m_thickness;
        m_vertices[idx * 8 + 6] = pos.x + m_thickness;
        m_vertices[idx * 8 + 7] = pos.y;
    }

    glm::ivec2 Snake::getSquarePosition(int idx)
    {
        return { m_vertices[idx * 8], m_vertices[idx * 8 + 1] };
    }

    glm::ivec4 Snake::getRectangle(int idx) const
    {
        return glm::ivec4(m_vertices[idx * 8], m_vertices[idx * 8 + 1], m_thickness, m_thickness);
    }

} // namespace Demo