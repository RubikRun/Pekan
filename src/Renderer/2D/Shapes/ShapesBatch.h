#pragma once

#include "Shape.h"
#include "Camera2D.h"
#include "RenderObject.h"
#include "RenderState.h"

#include <vector>

namespace Pekan
{
namespace Renderer
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

		// Adds a shape to the batch
		void addShape(const Shape& shape);

		// Renders all shapes from the batch
		void render(const Camera2DPtr& camera);
		void render();

		// Clears batch, removing all shapes, leaving it empty
		void clear();

	private:

		// Vertices of all shapes in the batch
		std::vector<ShapeVertex> m_vertices;

		// Indices of all shapes in the batch
		std::vector<unsigned> m_indices;

		// Underlying render object used for rendering all vertices and indices
		RenderObject m_renderObject;

		// Flag indicating if shapes batch is valid, meaning that it has been created and not yet destroyed
		bool m_isValid = false;
	};

} // namespace Renderer
} // namespace Pekan