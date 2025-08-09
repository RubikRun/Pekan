#pragma once

#include "Shape.h"
#include "RenderObject.h"
#include "Camera2D.h"

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
#include "Texture1D.h"
#endif

#include <vector>

namespace Pekan
{
namespace Renderer2D
{

	// A class representing a batch of 2D shapes.
	// Allows you to render many shapes at once in a single draw call.
	class ShapesBatch
	{
	public:

		// Creates an empty batch
		// @param[in] bufferDataUsage - Specifies the data usage of the underlying vertex buffer and index buffer
		void create(Graphics::BufferDataUsage bufferDataUsage);
		void destroy();

		// Adds a shape to the batch.
		// Returns true, if shape was successfully added to the batch.
		// If false is returned, it means that the shape was NOT added to the batch because it would overflow the batch.
		// In such case the batch needs to be rendered, cleared and the shape can be added to the next batch.
		bool addShape(const Shape& shape);

		// Renders all shapes from the batch with a given camera.
		//
		// NOTE: Camera can be null - then shapes will be rendered in default NDC (-1 to 1) space.
		void render(const Camera2D_ConstPtr& camera);

		// Clears batch, removing all shapes, leaving it empty
		void clear();

	private: /* functions */

		// Checks if adding a shape with given vertices count and indices count would overflow the batch
		bool wouldOverflow(int verticesCount, int indicesCount) const;

	private: /* variables */

		// Vertices of all shapes in the batch
		std::vector<ShapeVertex> m_vertices;

		// Indices of all shapes in the batch
		std::vector<unsigned> m_indices;

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		// Colors of all shapes in the batch
		std::vector<glm::vec4> m_colors;

		// Batch's capacity for colors
		int m_capacityColors = -1;
#endif
		// Batch's capacity for vertices
		int m_capacityVertices = -1;
		// Batch's capacity for indices
		int m_capacityIndices = -1;

		// Underlying render object used for rendering all vertices and indices
		RenderObject m_renderObject;

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		// Underlying 1D texture used for passing the colors of all shapes to the shader
		Graphics::Texture1D m_texture;

		// Number of shapes currently in the batch
		int m_shapesCount = 0;
#endif

		// Flag indicating if shapes batch is valid, meaning that it has been created and not yet destroyed
		bool m_isValid = false;
	};

} // namespace Renderer2D
} // namespace Pekan