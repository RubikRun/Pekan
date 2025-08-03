#pragma once

#include "Transformable2D.h"
#include "RenderObject.h"
#include "Image.h"
#include "Texture2D.h"

namespace Pekan
{
namespace Renderer2D
{

	struct SpriteVertex
	{
		// Position of the vertex, in world space
		glm::vec2 position = { 0.0f, 0.0f };
		// Texture coordinates of the vertex specifying a point in texture's UV space
		glm::vec2 textureCoordinates = { 0.0f, 0.0f };
		// Index of the texture to be used for the vertex
		float textureIndex = 0.0f;
	};

	class Sprite : public Transformable2D
	{
	public:

		void create(const Graphics::Image& image, float width, float height, bool dynamic = true);
		void destroy();

		// Checks if sprite is valid, meaning that it has been created and not yet destroyed
		inline bool isValid() const { return m_isValid; }

		void render() const;

		void setWidth(float width);
		void setHeight(float height);

		inline float getWidth() const { return m_width; }
		inline float getHeight() const { return m_height; }

		// Returns sprite's vertex data in world space.
		const SpriteVertex* getVertices() const;

		// TODO: temp
		int getTextureIndex() const { return m_textureIndex; }

		// TODO: temp
		const Graphics::Texture2D* getTexture() const { return &m_texture; }

		// Checks if sprite is dynamic,
		// meaning it will be changed/transformed often,
		// and it's better to use dynamic buffers for its vertices.
		inline bool isDynamic() const { return m_isDynamic; }

	private: /* functions */

		// Updates local vertices from current width and height
		void updateVerticesLocal() const;
		// Updates world vertices from current local vertices and current transform matrix
		void updateVerticesWorld() const;

	private: /* variables */

		// Sprite's underlying texture containing sprite's image
		Graphics::Texture2D m_texture;

		// TODO: temp
		int m_textureIndex = -1;

		// TODO: temp
		static int m_texturesCount;

		// Width of sprite, size across the X axis in local space
		float m_width = 0.0f;
		// Height of sprite, size across the Y acis in local space
		float m_height = 0.0f;

		// The 4 vertices (vertex positions) of the sprite, in local space
		mutable glm::vec2 m_verticesLocal[4] = {};
		// The 4 vertices of the sprite, in world space.
		mutable SpriteVertex m_verticesWorld[4];

		// Flag indicating if local vertices need to be updated before use
		mutable bool m_needUpdateVerticesLocal = true;
		// Flag indicating if world vertices need to be updated before use
		mutable bool m_needUpdateVerticesWorld = true;

		// Flag indicating if dynamic buffers should be used for sprite's vertices.
		// Set to true if sprite's vertices are going to be changed often.
		// Used for optimization.
		bool m_isDynamic = true;

		// Flag indicating if sprite is valid, meaning that it has been created and not yet destroyed
		bool m_isValid = false;
	};

} // namespace Renderer2D
} // namespace Pekan
