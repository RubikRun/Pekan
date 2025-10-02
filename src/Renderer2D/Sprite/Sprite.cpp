#include "Sprite.h"

#include "PekanLogger.h"
#include "Renderer2DSystem.h"

using namespace Pekan::Graphics;

namespace Pekan
{
namespace Renderer2D
{

	void Sprite::create(const Texture2D_ConstPtr& texture, float width, float height)
	{
        PK_ASSERT(!isValid(), "Trying to create a Sprite instance that is already created.", "Pekan");

        Transformable2D::_create();

        // Set default values to members
        // that will not be explicitly initialized here or updated later
        m_needUpdateVerticesLocal = true;
        m_needUpdateVerticesWorld = true;
        m_transformChangeIdUsedInVerticesWorld = 0;

        m_texture = texture;
        m_width = width;
        m_height = height;
        m_isValid = true;
	}

	void Sprite::destroy()
	{
        PK_ASSERT(isValid(), "Trying to destroy a Sprite instance that is not yet created.", "Pekan");

        Transformable2D::_destroy();

        m_isValid = false;
	}

    void Sprite::render() const
    {
        PK_ASSERT(m_isValid, "Trying to render a Sprite that is not yet created.", "Pekan");

        Renderer2DSystem::submitForRendering(*this);
    }

    void Sprite::setWidth(float width)
    {
        PK_ASSERT(isValid(), "Trying to set width of a Sprite that is not yet created.", "Pekan");
        PK_ASSERT(width >= 0.0f, "Sprite's width must be greater than or equal to 0.", "Pekan");

        m_width = width;
        m_needUpdateVerticesLocal = true;
    }

    void Sprite::setHeight(float height)
    {
        PK_ASSERT(isValid(), "Trying to set height of a Sprite that is not yet created.", "Pekan");
        PK_ASSERT(height >= 0.0f, "Sprite's height must be greater than or equal to 0.", "Pekan");

        m_height = height;
        m_needUpdateVerticesLocal = true;
    }

    void Sprite::setTexture(const Graphics::Texture2D_ConstPtr& texture)
    {
        PK_ASSERT(isValid(), "Trying to set height of a Sprite that is not yet created.", "Pekan");
        if (texture == nullptr)
        {
            PK_LOG_ERROR("Trying to set a null texture to a Sprite.", "Pekan");
            return;
        }

        m_texture = texture;
    }

    void Sprite::setTextureCoordinatesMin(glm::vec2 textureCoordinatesMin)
    {
        m_textureCoordinatesMin = textureCoordinatesMin;
        m_needUpdateVerticesWorld = true;
    }

    void Sprite::setTextureCoordinatesMax(glm::vec2 textureCoordinatesMax)
    {
        m_textureCoordinatesMax = textureCoordinatesMax;
        m_needUpdateVerticesWorld = true;
    }

    float Sprite::getWidth() const
    {
        PK_ASSERT(isValid(), "Trying to get width of a Sprite that is not yet created.", "Pekan");
        return m_width;
    }

    float Sprite::getHeight() const
    {
        PK_ASSERT(isValid(), "Trying to get height of a Sprite that is not yet created.", "Pekan");
        return m_height;
    }

    const Graphics::Texture2D_ConstPtr& Sprite::getTexture() const
    {
        PK_ASSERT(isValid(), "Trying to get the texture of a Sprite that is not yet created.", "Pekan");
        return m_texture;
    }

    glm::vec2 Sprite::getTextureCoordinatesMin()
    {
        PK_ASSERT(isValid(), "Trying to get the min texture coordinates of a Sprite that is not yet created.", "Pekan");
        return m_textureCoordinatesMin;
    }

    glm::vec2 Sprite::getTextureCoordinatesMax()
    {
        PK_ASSERT(isValid(), "Trying to get the max texture coordinates of a Sprite that is not yet created.", "Pekan");
        return m_textureCoordinatesMax;
    }

    const Vertex2D* Sprite::getVertices(float textureIndex) const
    {
        PK_ASSERT(isValid(), "Trying to get vertices of a Sprite that is not yet created.", "Pekan");
        PK_ASSERT(textureIndex >= 0.0f, "Trying to get vertices of a Sprite but giving it a negative texture index.", "Pekan");

        if (m_textureIndex != textureIndex)
        {
            m_textureIndex = textureIndex;
            m_needUpdateVerticesWorld = true;
        }

        if (m_transformChangeIdUsedInVerticesWorld < Transformable2D::getChangeId())
        {
            m_needUpdateVerticesWorld = true;
        }

        if (m_needUpdateVerticesLocal)
        {
            updateVerticesLocal();
        }
        if (m_needUpdateVerticesWorld)
        {
            updateVerticesWorld();
        }
        return m_verticesWorld;
    }

    void Sprite::updateVerticesLocal() const
    {
        PK_ASSERT(isValid(), "Trying to update local vertices of a Sprite that is not yet created.", "Pekan");

        // Use width and height to compute the 4 local vertex positions
        m_verticesLocal[0] = glm::vec2(-m_width / 2.0f, -m_height / 2.0f);
        m_verticesLocal[1] = glm::vec2(m_width / 2.0f, -m_height / 2.0f);
        m_verticesLocal[2] = glm::vec2(m_width / 2.0f, m_height / 2.0f);
        m_verticesLocal[3] = glm::vec2(-m_width / 2.0f, m_height / 2.0f);

        m_needUpdateVerticesLocal = false;
        m_needUpdateVerticesWorld = true;
    }

    void Sprite::updateVerticesWorld() const
    {
        PK_ASSERT(isValid(), "Trying to update world vertices of a Sprite that is not yet created.", "Pekan");

        const glm::mat3& worldMatrix = getWorldMatrix();
        // Calculate world vertex positions by applying the transform matrix to the local vertex positions
        m_verticesWorld[0].position = glm::vec2(worldMatrix * glm::vec3(m_verticesLocal[0], 1.0f));
        m_verticesWorld[1].position = glm::vec2(worldMatrix * glm::vec3(m_verticesLocal[1], 1.0f));
        m_verticesWorld[2].position = glm::vec2(worldMatrix * glm::vec3(m_verticesLocal[2], 1.0f));
        m_verticesWorld[3].position = glm::vec2(worldMatrix * glm::vec3(m_verticesLocal[3], 1.0f));

        // Set "textureCoordinates" attribute of each vertex
        PK_ASSERT
        (
            m_textureCoordinatesMin.x < m_textureCoordinatesMax.x
            && m_textureCoordinatesMin.y < m_textureCoordinatesMax.y,
            "Sprite has invalid texture coordinates. Default [0, 1] texture coordinates will be used for this Sprite.", "Pekan"
        );
        m_verticesWorld[0].textureCoordinates = { m_textureCoordinatesMin.x, m_textureCoordinatesMin.y };
        m_verticesWorld[1].textureCoordinates = { m_textureCoordinatesMax.x, m_textureCoordinatesMin.y };
        m_verticesWorld[2].textureCoordinates = { m_textureCoordinatesMax.x, m_textureCoordinatesMax.y };
        m_verticesWorld[3].textureCoordinates = { m_textureCoordinatesMin.x, m_textureCoordinatesMax.y };

        PK_ASSERT(m_textureIndex >= 0.0f, "Trying to update world vertices of a Sprite but texture index is negative.", "Pekan");

        // Set "textureIndex" attribute of each vertex
        m_verticesWorld[0].textureIndex = float(m_textureIndex);
        m_verticesWorld[1].textureIndex = float(m_textureIndex);
        m_verticesWorld[2].textureIndex = float(m_textureIndex);
        m_verticesWorld[3].textureIndex = float(m_textureIndex);

        // Cache change ID of the transform that we just used to update world vertices
        m_transformChangeIdUsedInVerticesWorld = Transformable2D::getChangeId();

        m_needUpdateVerticesWorld = false;
    }

} // namespace Renderer2D
} // namespace Pekan
