#include "Sprite.h"

#include "PekanLogger.h"

#define VERTEX_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shapes/Shaders/Sprite_VertexShader.glsl"
#define FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Sprite/Shaders/Sprite_FragmentShader.glsl"

using namespace Pekan::Graphics;

namespace Pekan
{
namespace Renderer2D
{

	void Sprite::create(const Image& image, float width, float height, bool dynamic)
	{
        PK_ASSERT(!isValid(), "Trying to create a Sprite instance that is already created.", "Pekan");

        // Set default values to members
        // that will not be explicitly initialized here or updated later
        m_needUpdateVerticesLocal = true;
        m_needUpdateVerticesWorld = true;

        // Create sprite's texture with the given image
        m_texture.create(image);

        m_width = width;
        m_height = height;
        m_isDynamic = dynamic;
        m_isValid = true;
	}

	void Sprite::destroy()
	{
        PK_ASSERT(isValid(), "Trying to destroy a Sprite instance that is not yet created.", "Pekan");

        m_isValid = false;
	}

    void Sprite::render() const
    {
        // TODO
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

    const SpriteVertex* Sprite::getVertices() const
    {
        PK_ASSERT(isValid(), "Trying to get vertices of a Sprite that is not yet created.", "Pekan");

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

        const glm::mat3& transformMatrix = getTransformMatrix();
        // Calculate world vertex positions by applying the transform matrix to the local vertex positions
        m_verticesWorld[0].position = glm::vec2(transformMatrix * glm::vec3(m_verticesLocal[0], 1.0f));
        m_verticesWorld[1].position = glm::vec2(transformMatrix * glm::vec3(m_verticesLocal[1], 1.0f));
        m_verticesWorld[2].position = glm::vec2(transformMatrix * glm::vec3(m_verticesLocal[2], 1.0f));
        m_verticesWorld[3].position = glm::vec2(transformMatrix * glm::vec3(m_verticesLocal[3], 1.0f));

        // Set "uvCoordinates" attribute of each vertex
        m_verticesWorld[0].uvCoordinates = { 0.0f, 0.0f };
        m_verticesWorld[1].uvCoordinates = { 1.0f, 0.0f };
        m_verticesWorld[2].uvCoordinates = { 1.0f, 1.0f };
        m_verticesWorld[3].uvCoordinates = { 0.0f, 1.0f };

        m_needUpdateVerticesWorld = false;
    }

} // namespace Renderer2D
} // namespace Pekan
