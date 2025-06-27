#include "Shape.h"

#include "Renderer2D.h"
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
        PK_ASSERT(m_isValid, "Trying to render a Shape that is not yet created.", "Pekan");

        Renderer2D::render(*this);
    }

    void Shape::setPosition(glm::vec2 position)
    {
        PK_ASSERT(m_isValid, "Trying to set position of a Shape that is not yet created.", "Pekan");

        m_position = position;
        m_needUpdateTransformMatrix = true;
        m_needUpdateVerticesWorld = true;
    }

    void Shape::setRotation(float rotation)
    {
        PK_ASSERT(m_isValid, "Trying to set rotation of a Shape that is not yet created.", "Pekan");

        m_rotation = rotation;
        m_needUpdateTransformMatrix = true;
        m_needUpdateVerticesWorld = true;
    }

    void Shape::setScale(glm::vec2 scale)
    {
        PK_ASSERT(m_isValid, "Trying to set position of a Shape that is not yet created.", "Pekan");

        m_scale = scale;
        m_needUpdateTransformMatrix = true;
        m_needUpdateVerticesWorld = true;
    }

    void Shape::setColor(glm::vec4 color)
    {
        PK_ASSERT(m_isValid, "Trying to set color of a Shape that is not yet created.", "Pekan");

        m_color = color;
        m_needUpdateVerticesWorld = true;
    }

    void Shape::move(glm::vec2 deltaPosition)
    {
        PK_ASSERT(m_isValid, "Trying to move a Shape that is not yet created.", "Pekan");

        m_position += deltaPosition;
        m_needUpdateTransformMatrix = true;
        m_needUpdateVerticesWorld = true;
    }

    void Shape::create()
    {
        PK_ASSERT(!isValid(), "Trying to create a Shape instance that is already created.", "Pekan");

        m_position = glm::vec2(0.0f, 0.0f);
        m_rotation = 0.0f;
        m_scale = glm::vec2(1.0f, 1.0f);
        m_isValid = true;
        m_needUpdateVerticesWorld = true;
        m_transformMatrix = glm::mat4(1.0f);
        m_needUpdateTransformMatrix = false;
    }

    void Shape::destroy()
    {
        PK_ASSERT(isValid(), "Trying to destroy a Shape instance that is not yet created.", "Pekan");

        m_isValid = false;
    }

    const glm::mat3& Shape::getTransformMatrix() const
    {
        if (m_needUpdateTransformMatrix)
        {
            updateTransformMatrix();
        }
        return m_transformMatrix;
    }

    void Shape::updateTransformMatrix() const
    {
        const float cosRot = cos(m_rotation);
        const float sinRot = sin(m_rotation);

        const glm::mat3 scaleMatrix = glm::mat3
        (
            glm::vec3(m_scale.x,  0.0f,       0.0f),
            glm::vec3(0.0f,       m_scale.y,  0.0f),
            glm::vec3(0.0f,       0.0f,       1.0f)
        );
        const glm::mat3 rotationMatrix = glm::mat3
        (
            glm::vec3(cosRot,  -sinRot,  0.0f),
            glm::vec3(sinRot,  cosRot,   0.0f),
            glm::vec3(0.0f,    0.0f,     1.0f)
        );
        const glm::mat3 translationMatrix = glm::mat3
        (
            glm::vec3(1.0f,          0.0f,          0.0f),
            glm::vec3(0.0f,          1.0f,          0.0f),
            glm::vec3(m_position.x,  m_position.y,  1.0f)
        );

        // Multiply translation, rotation and scale matrices to get the combined action in a single transform matrix
        // NOTE: Order of multiplication is important!
        m_transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;

        m_needUpdateTransformMatrix = false;
        m_needUpdateVerticesWorld = true;
    }

} // namespace Renderer
} // namespace Renderer