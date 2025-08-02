#include "Transformable2D.h"

#include "PekanLogger.h"

namespace Pekan
{
namespace Renderer2D
{

    void Transformable2D::setPosition(glm::vec2 position)
    {
        m_position = position;
        m_needUpdateTransformMatrix = true;
        onTransformChanged();
    }

    void Transformable2D::setRotation(float rotation)
    {
        m_rotation = rotation;
        m_needUpdateTransformMatrix = true;
        onTransformChanged();
    }

    void Transformable2D::setScale(glm::vec2 scale)
    {
        m_scale = scale;
        m_needUpdateTransformMatrix = true;
        onTransformChanged();
    }

    void Transformable2D::move(glm::vec2 deltaPosition)
    {
        m_position += deltaPosition;
        m_needUpdateTransformMatrix = true;
        onTransformChanged();
    }

    void Transformable2D::rotate(float deltaRotation)
    {
        m_rotation += deltaRotation;
        m_needUpdateTransformMatrix = true;
        onTransformChanged();
    }

    void Transformable2D::scale(glm::vec2 deltaScale)
    {
        m_scale *= deltaScale;
        m_needUpdateTransformMatrix = true;
        onTransformChanged();
    }

    const glm::mat3& Transformable2D::getTransformMatrix() const
    {
        if (m_needUpdateTransformMatrix)
        {
            updateTransformMatrix();
        }
        return m_transformMatrix;
    }

    void Transformable2D::_create()
    {
        // Set default values
        m_position = glm::vec2(0.0f, 0.0f);
        m_rotation = 0.0f;
        m_scale = glm::vec2(1.0f, 1.0f);
        m_transformMatrix = glm::mat3(1.0f);
        m_needUpdateTransformMatrix = false;
    }

    void Transformable2D::_destroy()
    {}

    void Transformable2D::updateTransformMatrix() const
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
    }

} // namespace Renderer2D
} // namespace Renderer2D