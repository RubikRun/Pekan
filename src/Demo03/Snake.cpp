#include "Snake.h"
#include "Utils/PekanUtils.h"

using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::ShaderDataType;
using Pekan::Renderer::BufferDataUsage;
using Pekan::PekanEngine;

static const char* vertexShaderFilePath = "resources/03_vertex_shader.glsl";
static const char* fragmentShaderFilePath = "resources/03_fragment_shader.glsl";

namespace Demo
{

    static const float THICKNESS = 0.07f;
    static const float INITIAL_POSITION_X = 0.0f;
    static const float INITIAL_POSITION_Y = 0.0f;
    // The snake will move by 1 square every MOVE_FRAMES frames
    static const int MOVE_FRAMES = 20;

	bool Snake::init()
	{
        m_aspectRatio = float(PekanEngine::getWindowWidth()) / float(PekanEngine::getWindowHeight());
        m_squaresCount = 4;
        m_headIdx = 0;
        m_tailIdx = 3;

        // Initialize vertices of cube
        m_vertices =
        {
            // square 0
            INITIAL_POSITION_X, INITIAL_POSITION_Y * m_aspectRatio, // top left
            INITIAL_POSITION_X, (INITIAL_POSITION_Y - THICKNESS) * m_aspectRatio, // bottom left
            INITIAL_POSITION_X + THICKNESS, (INITIAL_POSITION_Y - THICKNESS) * m_aspectRatio, // bottom right
            INITIAL_POSITION_X + THICKNESS, INITIAL_POSITION_Y * m_aspectRatio, // top right
            // square 1
            INITIAL_POSITION_X + THICKNESS, INITIAL_POSITION_Y * m_aspectRatio, // top left
            INITIAL_POSITION_X + THICKNESS, (INITIAL_POSITION_Y - THICKNESS) * m_aspectRatio, // bottom left
            INITIAL_POSITION_X + 2 * THICKNESS, (INITIAL_POSITION_Y - THICKNESS) * m_aspectRatio, // bottom right
            INITIAL_POSITION_X + 2 * THICKNESS, (INITIAL_POSITION_Y) * m_aspectRatio, // top right
            // square 2
            INITIAL_POSITION_X + 2 * THICKNESS, INITIAL_POSITION_Y * m_aspectRatio, // top left
            INITIAL_POSITION_X + 2 * THICKNESS, (INITIAL_POSITION_Y - THICKNESS) * m_aspectRatio, // bottom left
            INITIAL_POSITION_X + 3 * THICKNESS, (INITIAL_POSITION_Y - THICKNESS) * m_aspectRatio, // bottom right
            INITIAL_POSITION_X + 3 * THICKNESS, INITIAL_POSITION_Y * m_aspectRatio, // top right
            // square 3
            INITIAL_POSITION_X + 3 * THICKNESS, INITIAL_POSITION_Y * m_aspectRatio, // top left
            INITIAL_POSITION_X + 3 * THICKNESS, (INITIAL_POSITION_Y - THICKNESS) * m_aspectRatio, // bottom left
            INITIAL_POSITION_X + 4 * THICKNESS, (INITIAL_POSITION_Y - THICKNESS) * m_aspectRatio, // bottom right
            INITIAL_POSITION_X + 4 * THICKNESS, INITIAL_POSITION_Y * m_aspectRatio, // top right
        };

        m_indices =
        {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            8, 9, 10, 10, 11, 8,
            12, 13, 14, 14, 15, 12
        };

        // Create a vertex array
        m_vertexArray.create();

        // Create a vertex buffer with vertices data
        m_vertexBuffer.create(m_vertices.data(), m_vertices.size() * sizeof(float), BufferDataUsage::DynamicDraw);

        // Add vertex buffer to vertex array
        m_vertexArray.addVertexBuffer(m_vertexBuffer, { { ShaderDataType::Float2, "position" } });

        // Create an index buffer with indices data
        m_indexBuffer.create(m_indices.data(), m_indices.size() * sizeof(int));

        // Create a shader by compiling source code of vertex shader and fragment shader
        m_shader.create
        (
            Pekan::Utils::readFileToString(vertexShaderFilePath).c_str(),
            Pekan::Utils::readFileToString(fragmentShaderFilePath).c_str()
        );

        return true;
	}

    void Snake::update()
    {
        m_frames++;

        if (PekanEngine::isKeyPressed_W())
        {
            m_direction = { 0.0f, 1.0f };
        }
        else if (PekanEngine::isKeyPressed_A())
        {
            m_direction = { -1.0f, 0.0f };
        }
        else if (PekanEngine::isKeyPressed_S())
        {
            m_direction = { 0.0f, -1.0f };
        }
        else if (PekanEngine::isKeyPressed_D())
        {
            m_direction = { 1.0f, 0.0f };
        }

        if (m_frames % MOVE_FRAMES == 0)
        {
            move();
        }
    }

    void Snake::render()
    {
        PekanRenderer::clear();

        m_vertexArray.bind();
        m_shader.bind();

        // Draw snake
        PekanRenderer::drawIndexed(m_indices.size());

        m_shader.unbind();
        m_vertexArray.unbind();
    }

    void Snake::exit()
    {
        m_shader.destroy();
        m_indexBuffer.destroy();
        m_vertexBuffer.destroy();
        m_vertexArray.destroy();
    }

    void Snake::move()
    {
        setSquarePosition(m_tailIdx, getSquarePosition(m_headIdx) + m_direction * THICKNESS);
        m_headIdx = m_tailIdx;
        m_tailIdx -= 1;
        if (m_tailIdx < 0)
        {
            m_tailIdx = m_squaresCount - 1;
        }

        m_vertexBuffer.setData(m_vertices.data(), m_vertices.size() * sizeof(float), BufferDataUsage::DynamicDraw);
    }

    void Snake::setSquarePosition(int idx, glm::vec2 pos)
    {
        m_vertices[idx * 8 + 0] = pos.x;
        m_vertices[idx * 8 + 1] = pos.y * m_aspectRatio;
        m_vertices[idx * 8 + 2] = pos.x;
        m_vertices[idx * 8 + 3] = (pos.y - THICKNESS) * m_aspectRatio;
        m_vertices[idx * 8 + 4] = pos.x + THICKNESS;
        m_vertices[idx * 8 + 5] = (pos.y - THICKNESS) * m_aspectRatio;
        m_vertices[idx * 8 + 6] = pos.x + THICKNESS;
        m_vertices[idx * 8 + 7] = pos.y * m_aspectRatio;
    }

    glm::vec2 Snake::getSquarePosition(int idx)
    {
        return { m_vertices[idx * 8], m_vertices[idx * 8 + 1] / m_aspectRatio };
    }

} // namespace Demo