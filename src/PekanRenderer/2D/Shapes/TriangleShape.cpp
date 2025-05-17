#include "TriangleShape.h"

#include "Utils/PekanUtils.h"

#define VERTEX_SHADER_FILEPATH PEKAN_RENDERER_ROOT_DIR "/assets/shaders/VertexShader_2D.glsl"
#define FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER_ROOT_DIR "/assets/shaders/FragmentShader_SolidColor.glsl"

static constexpr glm::vec4 DEFAULT_COLOR = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
static constexpr unsigned FLOAT_SIZE = sizeof(float);
static constexpr unsigned VERTICES_SIZE = 6 * FLOAT_SIZE;

namespace Pekan
{
namespace Renderer
{

    void TriangleShape::create()
    {
        m_position = { 0.0f, 0.0f };

        m_vertices[0] = glm::vec2(0.0f, 0.0f);
        m_vertices[1] = glm::vec2(0.0f, 0.0f);
        m_vertices[2] = glm::vec2(0.0f, 0.0f);

        setColor(DEFAULT_COLOR);
    }

	void TriangleShape::create
    (
        glm::vec2 vertexA, glm::vec2 vertexB, glm::vec2 vertexC,
        bool dynamic
    )
	{
        m_position = { 0.0f, 0.0f };

        m_vertices[0] = vertexA;
        m_vertices[1] = vertexB;
        m_vertices[2] = vertexC;

        const BufferDataUsage vertexDataUsage = (dynamic ? BufferDataUsage::DynamicDraw : BufferDataUsage::StaticDraw);

        m_renderObject.create(
            m_vertices, VERTICES_SIZE,
            { { ShaderDataType::Float2, "position" } },
            vertexDataUsage,
            Utils::readFileToString(VERTEX_SHADER_FILEPATH).c_str(),
            Utils::readFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
        );

        setColor(DEFAULT_COLOR);
	}

    void TriangleShape::destroy()
    {
        PK_ASSERT(m_renderObject.isValid(), "Trying to destroy a TriangleShape that is not yet created.", "Pekan");
        m_renderObject.destroy();
    }

    void TriangleShape::render() const
    {
        PK_ASSERT(m_renderObject.isValid(), "Trying to render a TriangleShape that is not yet created.", "Pekan");
        m_renderObject.bind();
        PekanRenderer::draw(3);
    }

    void TriangleShape::setPosition(glm::vec2 position)
    {
        PK_ASSERT(m_renderObject.isValid(), "Trying to set position of a TriangleShape that is not yet created.", "Pekan");
        move(position - m_position);
    }

    void TriangleShape::move(glm::vec2 deltaPosition)
    {
        PK_ASSERT(m_renderObject.isValid(), "Trying to move a TriangleShape that is not yet created.", "Pekan");
        m_position += deltaPosition;

        m_vertices[0] += deltaPosition;
        m_vertices[1] += deltaPosition;
        m_vertices[2] += deltaPosition;

        m_renderObject.setVertexData(m_vertices, VERTICES_SIZE);
    }

    void TriangleShape::setColor(glm::vec4 color)
    {
        PK_ASSERT(m_renderObject.isValid(), "Trying to set color of a TriangleShape that is not yet created.", "Pekan");
        m_color = color;
        m_renderObject.getShader().setUniform4fv("uColor", color);
    }

} // namespace Renderer
} // namespace Renderer