#pragma once

#include "Transformable2D.h"
#include "RenderObject.h"
#include "Texture2D.h"
#include "Vertex2D.h"

namespace Pekan
{
namespace Renderer2D
{

	class Sprite : public Transformable2D
	{
	public:

		// Creates a sprite with a given texture and a given size
		void create(const Graphics::Texture2D_ConstPtr& texture, float width, float height);
		void destroy();

		// Submits sprite for rendering in Renderer2DSystem.
		// Actual rendering might happen at a later stage.
		void render() const;

		// Sets sprite's width, in local space
		void setWidth(float width);
		// Sets sprite's height, in local space
		void setHeight(float height);
		// Sets sprite's texture
		void setTexture(const Graphics::Texture2D_ConstPtr& texture);
		// Sets the min/max of sprite's texture coordinates
		// determining the rectangle in texture space
		// that this sprite maps to.
		// The default rectangle in texture space is [0, 0] to [1, 1],
		// which makes the sprite map to the texture exactly.
		// If the rectangle in texture space is [0, 0] to [3, 2] for example,
		// this will make the sprite repeat the texture 3 times horizontally, and 2 times vertically.
		// If the rectangle in texture space is [0.25, 0.25] to [0.75, 0.75] for example,
		// this will make the sprite show a zoomed in portion of the texture.
		void setTextureCoordinatesMin(glm::vec2 textureCoordinatesMin);
		void setTextureCoordinatesMax(glm::vec2 textureCoordinatesMax);

		// Returns sprite's width, in local space
		float getWidth() const;
		// Returns sprite's height, in local space
		float getHeight() const;
		// Returns (a pointer to) underlying texture
		const Graphics::Texture2D_ConstPtr& getTexture() const;
		// Returns the min/max of sprite's texture coordinates
		// determining the rectangle in texture space
		// that this sprite maps to.
		glm::vec2 getTextureCoordinatesMin();
		glm::vec2 getTextureCoordinatesMax();

		// Returns sprite's vertex data, in world space
		// @param[in] textureIndex - Index of sprite's texture inside of sprite's batch. Determines the value of the "textureIndex" attribute of sprite's vertices
		const Vertex2D* getVertices(float textureIndex) const;

		// Checks if sprite is valid, meaning that it has been created and not yet destroyed
		bool isValid() const { return m_isValid; }

	private: /* functions */

		// Updates local vertices from current width and height
		void updateVerticesLocal() const;
		// Updates world vertices from current local vertices and current transform matrix
		void updateVerticesWorld() const;

	private: /* variables */

		// Sprite's underlying texture containing sprite's image
		Graphics::Texture2D_ConstPtr m_texture;

		// Width of sprite, size across the X axis in local space
		float m_width = 0.0f;
		// Height of sprite, size across the Y acis in local space
		float m_height = 0.0f;

		// Min/max of sprite's texture coordinates,
		// determining the rectangle in texture space
		// that this sprite maps to.
		glm::vec2 m_textureCoordinatesMin = { 0.0f, 0.0f };
		glm::vec2 m_textureCoordinatesMax = { 1.0f, 1.0f };

		// The 4 vertices (vertex positions) of the sprite, in local space
		mutable glm::vec2 m_verticesLocal[4] = {};
		// The 4 vertices of the sprite, in world space.
		mutable Vertex2D m_verticesWorld[4];

		// Flag indicating if local vertices need to be updated before use
		mutable bool m_needUpdateVerticesLocal = true;
		// Flag indicating if world vertices need to be updated before use
		mutable bool m_needUpdateVerticesWorld = true;

		// Change ID of transform used in currently cached world vertices
		mutable unsigned m_transformChangeIdUsedInVerticesWorld = 0;

		// Sprite's texture's index inside of its batch.
		//
		// NOTE: Marked as "mutable" because it doesn't reflect a sprite's state exactly.
		// It reflects the state of "a sprite inside a batch".
		// We want to be able to change a sprite's index in a "const" way
		// because it doesn't change the actual sprite.
		// This can, of course, be avoided by letting a Sprite be "just a sprite"
		// and return only its actual vertices without any batch-related metadata,
		// but it would be inefficient, because then a batch will have to copy all vertices
		// in order to add the "textureIndex" attribute to them.
		mutable float m_textureIndex = -1.0f;

		// Flag indicating if sprite is valid, meaning that it has been created and not yet destroyed
		bool m_isValid = false;
	};

} // namespace Renderer2D
} // namespace Pekan
