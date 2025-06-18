#include "Shape.h"

#include "Utils/PekanUtils.h"

#define VERTEX_SHADER_FILEPATH PEKAN_RENDERER_ROOT_DIR "/shaders/VertexShader_2D.glsl"
#define FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER_ROOT_DIR "/shaders/FragmentShader_SolidColor.glsl"

static constexpr unsigned FLOAT_SIZE = sizeof(float);
static constexpr unsigned VERTICES_SIZE = 6 * FLOAT_SIZE;

namespace Pekan
{
namespace Renderer
{

    void Shape::render() const
    {
        PK_ASSERT(isValid(), "Trying to render a Shape that is not yet created.", "Pekan");
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

    void Shape::render(const Camera2D& camera)
    {
        PK_ASSERT(isValid(), "Trying to render a Shape that is not yet created.", "Pekan");
        m_renderObject.bind();

        // Set shader's view projection matrix uniform
        const glm::mat4& viewProjectionMatrix = camera.getViewProjectionMatrix();
        m_renderObject.getShader().setUniformMatrix4fv("u_viewProjectionMatrix", viewProjectionMatrix);

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
        PK_ASSERT(isValid(), "Trying to set position of a Shape that is not yet created.", "Pekan");
        _moveVertices(position - m_position);
        m_position = position;
    }

    void Shape::move(glm::vec2 deltaPosition)
    {
        PK_ASSERT(isValid(), "Trying to move a Shape that is not yet created.", "Pekan");
        _moveVertices(deltaPosition);
        m_position += deltaPosition;
    }

    void Shape::setColor(glm::vec4 color)
    {
        PK_ASSERT(isValid(), "Trying to set color of a Shape that is not yet created.", "Pekan");
        m_color = color;
        m_renderObject.getShader().setUniform4fv("uColor", color);
    }

    void Shape::createRenderObject(bool dynamic)
    {
        if (isValid())
        {
            PK_LOG_WARNING("Creating a 2D shape, but this Shape instance has been created before,"
                " there is a valid render object inside. This old render object will be destroyed.", "Pekan");
            destroyRenderObject();
        }

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

        // Set shader's view projection matrix uniform to an identity matrix
        static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
        m_renderObject.getShader().setUniformMatrix4fv("u_viewProjectionMatrix", defaultViewProjectionMatrix);

        setColor(m_color);
    }

    void Shape::destroyRenderObject()
    {
        PK_ASSERT(isValid(), "Trying to destroy a Shape that is not yet created.", "Pekan");
        m_renderObject.destroy();
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