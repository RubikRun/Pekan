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
		// Returns true, if sprite was successfully added to the batch.
		// If false is returned, it means that the sprite was NOT added to the batch because it would overflow the batch.
		// In such case the batch needs to be rendered, cleared and the sprite can be added to the next batch.
		bool addSprite(const Sprite& sprite);

		// Renders all sprites from the batch with a given camera.
		//
		// NOTE: Camera can be null - then sprites will be rendered in default NDC (-1 to 1) space.
		void render(const Camera2D_ConstPtr& camera);

		// Clears batch, removing all sprites, leaving it empty
		void clear();

	private: /* functions */

		// Checks if adding one more sprite would overflow the batch
		bool wouldOverflow() const;

	private: /* variables */

		// Vertices of all sprites in the batch
		std::vector<SpriteVertex> m_vertices;

		// Indices of all sprites in the batch
		std::vector<unsigned> m_indices;

		// Textures of all sprites in the batch
		std::vector<Graphics::Texture2D_ConstPtr> m_textures;

		// Underlying render object used for rendering all vertices and indices with all textures attached
		RenderObject m_renderObject;

		// Batch's capacity for vertices
		int m_capacityVertices = -1;
		// Batch's capacity for indices
		int m_capacityIndices = -1;
		// Batch's capacity for textures
		int m_capacityTextures = -1;

		// Flag indicating if sprites batch is valid, meaning that it has been created and not yet destroyed
		bool m_isValid = false;
	};

} // namespace Renderer2D
} // namespace Pekan