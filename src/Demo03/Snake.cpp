#include "Snake.h"
#include "Utils/PekanUtils.h"

using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::ShaderDataType;
using Pekan::Renderer::BufferDataUsage;
using Pekan::PekanEngine;
using Pekan::KeyCode;

static const char* vertexShaderFilePath = "resources/03_snake_vertexShader.glsl";
static const char* fragmentShaderFilePath = "resources/03_snake_fragmentShader.glsl";
static const float THICKNESS = 0.05f;
static const float INITIAL_POSITION_X = 0.5f;
static const float INITIAL_POSITION_Y = 0.5f;
static const int INITIAL_SPEED = 20;

static const int SPEED_MAP[] =
{
    20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 11, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7,
    6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

namespace Demo
{
    static bool checkOverlapRectangles(glm::vec4 r1, glm::vec4 r2)
    {
        return (!(r2.x + r2.z < r1.x || r2.x > r1.x + r1.z || r2.y + r2.w < r1.y || r2.y > r1.y + r1.w));
    }

    static bool isRectangleInsideOfWindow(glm::vec4 r1, glm::ivec2 resolution)
    {
        return (r1.x >= 0 && r1.y >= 0 && r1.x + r1.z - 1 < resolution.x && r1.y + r1.w - 1 < resolution.y);
    }

	bool Snake::create()
	{
        m_resolution = PekanEngine::getWindowResolution();

        m_speedIdx = 0;

        m_squaresCount = 4;
        m_headIdx = 0;
        m_tailIdx = 3;

        m_thickness = THICKNESS * m_resolution.y;
        const int posX = int(INITIAL_POSITION_X * m_resolution.x);
        const int posY = int(INITIAL_POSITION_Y * m_resolution.y);

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
            BufferDataUsage::DynamicDraw,
            Pekan::Utils::readFileToString(vertexShaderFilePath).c_str(),
            Pekan::Utils::readFileToString(fragmentShaderFilePath).c_str()
        );

        m_renderObject.getShader().setUniform2fv("uResolution", glm::vec2(float(m_resolution.x), float(m_resolution.y)));

        return true;
	}

    void Snake::update()
    {
        if (m_running)
        {
            m_frames++;

            if (PekanEngine::isKeyPressed(KeyCode::KEY_W))
            {
                m_direction = { 0, -1 };
            }
            else if (PekanEngine::isKeyPressed(KeyCode::KEY_A))
            {
                m_direction = { -1, 0 };
            }
            else if (PekanEngine::isKeyPressed(KeyCode::KEY_S))
            {
                m_direction = { 0, 1 };
            }
            else if (PekanEngine::isKeyPressed(KeyCode::KEY_D))
            {
                m_direction = { 1, 0 };
            }

            if (m_frames % (m_speedIdx < 77 ? SPEED_MAP[m_speedIdx] : 1) == 0)
            {
                move();
            }
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

    void Snake::grow()
    {
        m_shouldGrow = true;
    }

    void Snake::move()
    {
        if (m_shouldGrow)
        {
            moveGrowing();
            m_shouldGrow = false;
        }
        else
        {
            moveNormally();
        }
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

    glm::ivec2 Snake::getSquarePosition(int idx) const
    {
        return { m_vertices[idx * 8], m_vertices[idx * 8 + 1] };
    }

    glm::ivec4 Snake::getRectangle(int idx) const
    {
        return glm::ivec4(m_vertices[idx * 8], m_vertices[idx * 8 + 1], m_thickness, m_thickness);
    }

    void Snake::moveGrowing()
    {
        // Add an invalid square at the end
        m_squaresCount++;
        for (int i = 0; i < 8; i++) { m_vertices.push_back(-1); };
        // Move squares [m_headIdx, ... , m_squaresCount - 2] to [m_headIdx + 1, ... , m_squaresCount - 1]
        // This opens up a free square at m_headIdx
        for (int i = m_squaresCount - 1; i > m_headIdx; i--)
        {
            setSquarePosition(i, getSquarePosition(i - 1));
        }
        // where we can put the new square which is the result of moving the old head
        setSquarePosition(m_headIdx, getSquarePosition(m_headIdx + 1) + m_direction * m_thickness);
        // In the special case where m_headIdx is 0 then we had m_tailIdx being the last element,
        // and now after insterting a new last element we need to change m_tailIdx to point to that new last element
        if (m_headIdx == 0)
        {
            m_tailIdx = m_squaresCount - 1;
        }

        m_indices.push_back((m_squaresCount - 1) * 4 + 0);
        m_indices.push_back((m_squaresCount - 1) * 4 + 1);
        m_indices.push_back((m_squaresCount - 1) * 4 + 2);
        m_indices.push_back((m_squaresCount - 1) * 4 + 2);
        m_indices.push_back((m_squaresCount - 1) * 4 + 3);
        m_indices.push_back((m_squaresCount - 1) * 4 + 0);

        m_renderObject.setIndexData(m_indices.data(), m_indices.size() * sizeof(int), BufferDataUsage::DynamicDraw);
        m_renderObject.setVertexData(m_vertices.data(), m_vertices.size() * sizeof(int), BufferDataUsage::DynamicDraw);

        m_speedIdx++;
    }

    void Snake::moveNormally()
    {
        setSquarePosition(m_tailIdx, getSquarePosition(m_headIdx) + m_direction * m_thickness);
        m_headIdx = m_tailIdx;
        m_tailIdx -= 1;
        if (m_tailIdx < 0)
        {
            m_tailIdx = m_squaresCount - 1;
        }
        m_renderObject.setVertexData(m_vertices.data(), m_vertices.size() * sizeof(int), BufferDataUsage::DynamicDraw);

        if (!isRectangleInsideOfWindow(getRectangle(m_headIdx), m_resolution) || isBitingItself())
        {
            m_running = false;
        }
    }

    bool Snake::isBitingItself() const
    {
        const glm::ivec2 headPos = getSquarePosition(m_headIdx);
        for (int i = 0; i < m_squaresCount; i++)
        {
            if (i == m_headIdx)
            {
                continue;
            }
            // We don't need to check if head's square overlaps with body's square,
            // because if they overlap they will defeinitely be on the same position,
            // because of the way snake moves.
            if (getSquarePosition(i) == headPos)
            {
                return true;
            }
        }
        return false;
    }

} // namespace Demo