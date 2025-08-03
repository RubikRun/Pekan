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

	// TODO: temp
	struct TextureBinding
	{
		int slot;
		const Graphics::Texture2D* texture;
	};

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
		void render(const Camera2D_ConstPtr& camera);
		void render();

		// Clears batch, removing all sprites, leaving it empty
		void clear();

	private:

		// Vertices of all sprites in the batch
		std::vector<SpriteVertex> m_vertices;

		// Indices of all sprites in the batch
		std::vector<unsigned> m_indices;

		// TODO: temp
		std::vector<TextureBinding> m_textureBindings;

		// Underlying render object used for rendering all vertices and indices
		RenderObject m_renderObject;

		// Flag indicating if sprites batch is valid, meaning that it has been created and not yet destroyed
		bool m_isValid = false;
	};

} // namespace Renderer2D
} // namespace Pekan