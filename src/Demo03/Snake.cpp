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

    static const float THICKNESS = 0.1f;
    static const float INITIAL_POSITION_X = 0.0f;
    static const float INITIAL_POSITION_Y = 0.0f;
    // The snake will move by 1 square every MOVE_FRAMES frames
    static const int MOVE_FRAMES = 20;

	bool Snake::init()
	{
        m_squaresCount = 4;

        // Initialize vertices of cube
        m_vertices =
        {
            // square 0
            INITIAL_POSITION_X, INITIAL_POSITION_Y, // top left
            INITIAL_POSITION_X, INITIAL_POSITION_Y - THICKNESS, // bottom left
            INITIAL_POSITION_X + THICKNESS, INITIAL_POSITION_Y - THICKNESS, // bottom right
            INITIAL_POSITION_X + THICKNESS, INITIAL_POSITION_Y, // top right
            // square 1
            INITIAL_POSITION_X + THICKNESS, INITIAL_POSITION_Y, // top left
            INITIAL_POSITION_X + THICKNESS, INITIAL_POSITION_Y - THICKNESS, // bottom left
            INITIAL_POSITION_X + 2 * THICKNESS, INITIAL_POSITION_Y - THICKNESS, // bottom right
            INITIAL_POSITION_X + 2 * THICKNESS, INITIAL_POSITION_Y, // top right
            // square 2
            INITIAL_POSITION_X + 2 * THICKNESS, INITIAL_POSITION_Y, // top left
            INITIAL_POSITION_X + 2 * THICKNESS, INITIAL_POSITION_Y - THICKNESS, // bottom left
            INITIAL_POSITION_X + 3 * THICKNESS, INITIAL_POSITION_Y - THICKNESS, // bottom right
            INITIAL_POSITION_X + 3 * THICKNESS, INITIAL_POSITION_Y, // top right
            // square 3
            INITIAL_POSITION_X + 3 * THICKNESS, INITIAL_POSITION_Y, // top left
            INITIAL_POSITION_X + 3 * THICKNESS, INITIAL_POSITION_Y - THICKNESS, // bottom left
            INITIAL_POSITION_X + 4 * THICKNESS, INITIAL_POSITION_Y - THICKNESS, // bottom right
            INITIAL_POSITION_X + 4 * THICKNESS, INITIAL_POSITION_Y, // top right
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
            m_direction = { 0, 1 };
        }
        else if (PekanEngine::isKeyPressed_A())
        {
            m_direction = { -1, 0 };
        }
        else if (PekanEngine::isKeyPressed_S())
        {
            m_direction = { 0, -1 };
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
        for (int i = 0; i < m_squaresCount; i++)
        {
            moveSquare(i);
        }

        m_vertexBuffer.setData(m_vertices.data(), m_vertices.size() * sizeof(float), BufferDataUsage::DynamicDraw);
    }

    void Snake::moveSquare(int idx)
    {
        const int first = idx * 8;
        for (int i = 0; i < 8; i++)
        {
            if (i % 2 == 0)
            {
                m_vertices[first + i] += float(m_direction.x) * THICKNESS;
            }
            else
            {
                m_vertices[first + i] += float(m_direction.y) * THICKNESS;
            }
        }
    }

} // namespace Demo