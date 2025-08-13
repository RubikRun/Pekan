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
		// @param[in] dynamic - Indicates if sprite will be changed/transformed often. Used for optimization.
		void create(const Graphics::Texture2D_ConstPtr& texture, float width, float height, bool dynamic = true);
		void destroy();

		// Checks if sprite is valid, meaning that it has been created and not yet destroyed
		bool isValid() const { return m_isValid; }

		// Submits sprite for rendering in Renderer2DSystem.
		// Actual rendering might happen at a later stage.
		void render() const;

		// Sets sprite's width, in local space
		void setWidth(float width);
		// Sets sprite's height, in local space
		void setHeight(float height);

		// Returns sprite's width, in local space
		float getWidth() const { return m_width; }
		// Returns sprite's height, in local space
		float getHeight() const { return m_height; }

		// Returns sprite's vertex data, in world space
		const Vertex2D* getVertices() const;

		// Returns (a pointer to) underlying texture
		const Graphics::Texture2D_ConstPtr& getTexture() const { return m_texture; }

		// Checks if sprite is dynamic,
		// meaning it will be changed/transformed often,
		// and it's better to use dynamic buffers for its vertices and indices.
		bool isDynamic() const { return m_isDynamic; }

		// Sets sprite's texture's index inside of its batch.
		// This index will determine the value of the "textureIndex" attribute of sprite's vertices
		void setTextureIndex(float textureIndex) const { m_textureIndex = textureIndex; }

	private: /* functions */

		// Updates local vertices from current width and height
		void updateVerticesLocal() const;
		// Updates world vertices from current local vertices and current transform matrix
		void updateVerticesWorld() const;

		// Called by base class Transformable2D when transform changes
		void onTransformChanged() override;

	private: /* variables */

		// Sprite's underlying texture containing sprite's image
		Graphics::Texture2D_ConstPtr m_texture;

		// Width of sprite, size across the X axis in local space
		float m_width = 0.0f;
		// Height of sprite, size across the Y acis in local space
		float m_height = 0.0f;

		// The 4 vertices (vertex positions) of the sprite, in local space
		mutable glm::vec2 m_verticesLocal[4] = {};
		// The 4 vertices of the sprite, in world space.
		mutable Vertex2D m_verticesWorld[4];

		// Flag indicating if local vertices need to be updated before use
		mutable bool m_needUpdateVerticesLocal = true;
		// Flag indicating if world vertices need to be updated before use
		mutable bool m_needUpdateVerticesWorld = true;

		// Flag indicating if dynamic buffers should be used for sprite's vertices.
		// Set to true if sprite's vertices are going to be changed often.
		// Used for optimization.
		bool m_isDynamic = true;

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
