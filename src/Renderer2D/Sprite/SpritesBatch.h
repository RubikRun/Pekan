#pragma once

#include "Sprite.h"
#include "RenderObject.h"
#include "Camera2D.h"
#include "Texture2D.h"

#include <vector>

namespace Pekan
{
namespace Renderer2D
{

	// A class representing a batch of 2D sprites.
	// Allows you to render many sprites at once in a single draw call.
	class SpritesBatch
	{
	public:

		// Creates an empty batch
		// @param[in] bufferDataUsage - Specifies the data usage of the underlying vertex buffer and index buffer
		void create(Graphics::BufferDataUsage bufferDataUsage);
		void destroy();

		// Adds a sprite to the batch.
		// @return true if we can continue adding sprites, not rendering yet.
		//         false if we need to render the batch, and start a new one.
		bool addSprite(const Sprite& sprite);

		// Renders all sprites from the batch
		// @param[in] isFinal - A flag indicating if this will be the final call to render() for this frame
		void render(const Camera2D_ConstPtr& camera, bool isFinal = false);
		void render(bool isFinal = false);

		// Clears batch, removing all sprites, leaving it empty
		// @param[in] isFinal - A flag indicating if this will be the final call to clear() for this frame
		void clear(bool isFinal = false);

	private: /* functions */

		// Renders carry-over sprites
		void renderCarryOver(const Camera2D_ConstPtr& camera);
		void renderCarryOver();

	private: /* variables */

		// Vertices of all sprites in the batch
		std::vector<SpriteVertex> m_vertices;

		// Indices of all sprites in the batch
		std::vector<unsigned> m_indices;

		// Textures of all sprites in the batch
		std::vector<Graphics::Texture2D_ConstPtr> m_textures;

		// List of carry-over vertices that couldn't be added to this batch but must be added to the next one
		std::vector<SpriteVertex> m_carryOverVertices;

		// List of carry-over indices that couldn't be added to this batch but must be added to the next one
		std::vector<unsigned> m_carryOverIndices;

		// List of carry-over textures that couldn't be added to this batch but must be added to the next one
		std::vector<Graphics::Texture2D_ConstPtr> m_carryOverTextures;

		// Underlying render object used for rendering all vertices and indices with all textures attached
		RenderObject m_renderObject;

		// Flag indicating if sprites batch is valid, meaning that it has been created and not yet destroyed
		bool m_isValid = false;
	};

} // namespace Renderer2D
} // namespace Pekan