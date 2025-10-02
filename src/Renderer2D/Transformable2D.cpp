#include "Transformable2D.h"

#include "PekanLogger.h"
#include "glm/gtc/epsilon.hpp"

namespace Pekan
{
namespace Renderer2D
{

    void Transformable2D::setParent(const Transformable2D* parent)
    {
        m_parent = parent;
        registerParentChange();
    }

    void Transformable2D::setPosition(glm::vec2 position)
    {
        m_position = position;
        registerLocalChange();
    }

    void Transformable2D::setRotation(float rotation)
    {
        m_rotation = rotation;
        registerLocalChange();
    }

    void Transformable2D::setScale(glm::vec2 scale)
    {
        m_scale = scale;
        registerLocalChange();
    }

    void Transformable2D::move(glm::vec2 deltaPosition)
    {
        m_position += deltaPosition;
        registerLocalChange();
    }

    void Transformable2D::rotate(float deltaRotation)
    {
        m_rotation += deltaRotation;
        registerLocalChange();
    }

    void Transformable2D::scale(glm::vec2 deltaScale)
    {
        m_scale *= deltaScale;
        registerLocalChange();
    }

    glm::vec2 Transformable2D::getPositionInWorld() const
    {
        if (m_parent == nullptr)
        {
            return m_position;
        }
        const glm::mat3& parentWorldMatrix = m_parent->getWorldMatrix();
        const glm::vec2 positionInWorld = glm::vec2(parentWorldMatrix * glm::vec3(m_position, 1.0f));
        return positionInWorld;
    }

    float Transformable2D::getRotationInWorld() const
    {
        if (m_parent == nullptr)
        {
            return m_rotation;
        }
        const float rotationInWorld = m_parent->getRotationInWorld() + m_rotation;
        return rotationInWorld;
    }

    glm::vec2 Transformable2D::getScaleInWorld() const
    {
        const glm::mat3& worldMatrix = getWorldMatrix();

        const glm::vec2 xAxis = glm::vec2(worldMatrix[0][0], worldMatrix[1][0]);
        const glm::vec2 yAxis = glm::vec2(worldMatrix[0][1], worldMatrix[1][1]);
        const float scaleX = glm::length(xAxis);
        const float scaleY = glm::length(yAxis);

        return { scaleX, scaleY };
    }

    const glm::mat3& Transformable2D::getLocalMatrix() const
    {
        if (m_isDirtyLocalMatrix)
        {
            updateLocalMatrix();
        }
        return m_localMatrix;
    }

    const glm::mat3& Transformable2D::getWorldMatrix() const
    {
        PK_ASSERT_QUICK(!(m_isDirtyLocalMatrix && !m_isDirtyWorldMatrix));

        // If parent has changed since the last time we updated our world matrix
        if (!m_isDirtyWorldMatrix && m_parent != nullptr
            && m_parentChangeIdUsedInWorldMatrix < m_parent->getChangeId())
        {
            // then we'll need to update our world matrix again
            m_isDirtyWorldMatrix = true;
        }

        if (m_isDirtyWorldMatrix)
        {
            updateWorldMatrix();
        }
        return m_worldMatrix;
    }

    unsigned Transformable2D::getChangeId() const
    {
        if (m_parent != nullptr)
        {
            const unsigned parentChangeId = m_parent->getChangeId();
            // If parent has changed since the last time we updated our change ID
            if (m_cachedParentChangeId < parentChangeId)
            {
                // then update our change ID again, and cache parent's new change ID
                m_changeId++;
                m_cachedParentChangeId = parentChangeId;
            }
        }

        return m_changeId;
    }

    void Transformable2D::_create()
    {
        // Set default values
        m_parent = nullptr;
        m_position = glm::vec2(0.0f, 0.0f);
        m_rotation = 0.0f;
        m_scale = glm::vec2(1.0f, 1.0f);
        m_changeId = 0;
        m_cachedParentChangeId = 0;
        m_localMatrix = glm::mat3(1.0f);
        m_worldMatrix = glm::mat3(1.0f);
        m_isDirtyLocalMatrix = true;
        m_isDirtyWorldMatrix = true;
    }

    void Transformable2D::_destroy()
    {}

    void Transformable2D::updateLocalMatrix() const
    {
        const float cosRot = cos(m_rotation);
        const float sinRot = sin(m_rotation);

        const glm::mat3 scaleMatrix = glm::mat3
        (
            glm::vec3(m_scale.x,    0.0f,         0.0f),
            glm::vec3(0.0f,         m_scale.y,    0.0f),
            glm::vec3(0.0f,         0.0f,         1.0f)
        );
        const glm::mat3 rotationMatrix = glm::mat3
        (
            glm::vec3(cosRot,    -sinRot,    0.0f),
            glm::vec3(sinRot,     cosRot,    0.0f),
            glm::vec3(0.0f,       0.0f,      1.0f)
        );
        const glm::mat3 translationMatrix = glm::mat3
        (
            glm::vec3(1.0f,            0.0f,            0.0f),
            glm::vec3(0.0f,            1.0f,            0.0f),
            glm::vec3(m_position.x,    m_position.y,    1.0f)
        );

        m_localMatrix = translationMatrix * rotationMatrix * scaleMatrix;

        m_isDirtyLocalMatrix = false;
    }

    void Transformable2D::updateWorldMatrix() const
    {
        const glm::mat3& localMatrix = getLocalMatrix();

        if (m_parent != nullptr)
        {
            const glm::mat3& parentWorldMatrix = m_parent->getWorldMatrix();
            m_worldMatrix = parentWorldMatrix * localMatrix;

            m_parentChangeIdUsedInWorldMatrix = m_parent->getChangeId();
        }
        else
        {
            m_worldMatrix = localMatrix;
        }

        m_isDirtyWorldMatrix = false;
    }

    void Transformable2D::registerLocalChange() const
    {
        m_changeId++;
        m_isDirtyLocalMatrix = true;
        m_isDirtyWorldMatrix = true;
    }

    void Transformable2D::registerParentChange() const
    {
        m_changeId++;
        m_isDirtyWorldMatrix = true;
    }

} // namespace Renderer2D
} // namespace Pekan