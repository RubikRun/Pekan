#pragma once

#include "Shape.h"
#include "Sprite.h"
#include "RenderObject.h"
#include "Camera2D.h"
#include "Texture2D.h"

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
#include "Texture1D.h"
#endif

#include <vector>

namespace Pekan
{
namespace Renderer2D
{

	// A batch of 2D primitives.
	// Allows you to render many primitives at once in a single draw call.
	class RenderBatch2D
	{
	public:

		// Creates an empty batch
		void create();
		void destroy();

		// Adds a shape to the batch.
		// Returns true, if shape was successfully added to the batch.
		// If false is returned, it means that the shape was NOT added to the batch because it would overflow the batch.
		// In such case the batch needs to be rendered, cleared and the shape can be added to the next batch.
		bool addShape(const Shape& shape);
		// Adds a sprite to the batch.
		// Returns true, if sprite was successfully added to the batch.
		// If false is returned, it means that the sprite was NOT added to the batch because it would overflow the batch.
		// In such case the batch needs to be rendered, cleared and the sprite can be added to the next batch.
		bool addSprite(const Sprite& sprite);

		// Renders all primitives from the batch with a given camera.
		//
		// NOTE: Camera can be null - then primitives will be rendered in default NDC (-1 to 1) space.
		void render(const Camera2D_ConstPtr& camera);

		// Clears batch, removing all primitives, leaving it empty
		void clear();

	private: /* functions */

		// Checks if adding a shape with given vertices count and indices count would overflow the batch
		bool wouldShapeOverflowBatch(int verticesCount, int indicesCount) const;
		// Checks if adding a sprite would overflow the batch
		bool wouldSpriteOverflowBatch() const;

	private: /* variables */

		// Vertices of all primitives in the batch
		std::vector<Vertex2D> m_vertices;
		// Indices of all primitives in the batch
		std::vector<unsigned> m_indices;
		// Textures of all primitives in the batch
		std::vector<Graphics::Texture2D_ConstPtr> m_textures;
#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		// Colors of all primitives in the batch
		std::vector<glm::vec4> m_colors;
#endif

		// Batch's capacity for textures
		int m_capacityTextures = -1;
#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		// Batch's capacity for colors
		int m_capacityColors = -1;
#endif

		// Underlying render object used for rendering all vertices and indices
		Graphics::RenderObject m_renderObject;
#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		// Underlying 1D texture used for passing the colors of all shapes to the shader
		Graphics::Texture1D m_colorsTexture;

		// Number of colors (of shapes) currently in the batch
		int m_colorsCount = 0;
#endif

		// Flag indicating if shapes batch is valid, meaning that it has been created and not yet destroyed
		bool m_isValid = false;
	};

} // namespace Renderer2D
} // namespace Pekan