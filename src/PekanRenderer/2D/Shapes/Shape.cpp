#include "Shape.h"

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

    void Shape::create()
    {
        m_position = { 0.0f, 0.0f };
        m_color = DEFAULT_COLOR;
    }

    void Shape::destroy()
    {
        PK_ASSERT(m_renderObject.isValid(), "Trying to destroy a Shape that is not yet created.", "Pekan");
        m_renderObject.destroy();
    }

    void Shape::render() const
    {
        PK_ASSERT(m_renderObject.isValid(), "Trying to render a Shape that is not yet created.", "Pekan");
        m_renderObject.bind();
        if (m_usingIndices)
        {
            PekanRenderer::drawIndexed((getNumberOfVertices() - 2) * 3, getDrawMode());
        }
        else
        {
            PekanRenderer::draw(getNumberOfVertices(), getDrawMode());
        }
    }

    void Shape::setPosition(glm::vec2 position)
    {
        PK_ASSERT(m_renderObject.isValid(), "Trying to set position of a Shape that is not yet created.", "Pekan");
        _moveVertices(position - m_position);
        m_position = position;
    }

    void Shape::move(glm::vec2 deltaPosition)
    {
        PK_ASSERT(m_renderObject.isValid(), "Trying to move a Shape that is not yet created.", "Pekan");
        _moveVertices(deltaPosition);
        m_position += deltaPosition;
    }

    void Shape::setColor(glm::vec4 color)
    {
        PK_ASSERT(m_renderObject.isValid(), "Trying to set color of a Shape that is not yet created.", "Pekan");
        m_color = color;
        m_renderObject.getShader().setUniform4fv("uColor", color);
    }

    void Shape::createRenderObject(bool dynamic)
    {
        const BufferDataUsage vertexDataUsage = (dynamic ? BufferDataUsage::DynamicDraw : BufferDataUsage::StaticDraw);

        // Create render object with vertex data and shaders
        m_renderObject.create
        (
            getVertexData(), getVertexDataSize(),
            { { ShaderDataType::Float2, "position" } }, vertexDataUsage,
            Utils::readFileToString(VERTEX_SHADER_FILEPATH).c_str(),
            Utils::readFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
        );
        // If derived class has index data, set index data as well
        const unsigned* indexData = getIndexData();
        m_usingIndices = (indexData != nullptr);
        if (m_usingIndices)
        {
            m_renderObject.setIndexData(indexData, getIndexDataSize(), BufferDataUsage::StaticDraw);
        }

        setColor(m_color);
    }

    void Shape::updateRenderObject()
    {
        m_renderObject.setVertexData(getVertexData(), getVertexDataSize());
    }

    void Shape::updateRenderObject(const void* indexData)
    {
        m_renderObject.setVertexData(getVertexData(), getVertexDataSize());
        m_renderObject.setIndexData(indexData, getIndexDataSize(), BufferDataUsage::DynamicDraw);
    }

} // namespace Renderer
} // namespace Renderer