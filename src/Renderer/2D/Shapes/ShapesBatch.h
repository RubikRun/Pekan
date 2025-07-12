#pragma once

#include "Shape.h"
#include "Camera2D.h"
#include "RenderObject.h"
#include "RenderState.h"
#include "Texture1D.h"

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

		// Adds a shape to the batch.
		// @return true if we can continue adding shapes, not rendering yet.
		//         false if we need to render the batch, and start a new one.
		bool addShape(const Shape& shape);

		// Renders all shapes from the batch
		void render(const Camera2D_ConstPtr& camera);
		void render();

		// Clears batch, removing all shapes, leaving it empty
		void clear();

	private:

		// Vertices of all shapes in the batch
		std::vector<ShapeVertex> m_vertices;

		// Indices of all shapes in the batch
		std::vector<unsigned> m_indices;

		// Colors of all shapes in the batch
		std::vector<glm::vec4> m_colors;

		// Number of shapes currently in the batch
		int m_shapesCount = 0;

		// Underlying render object used for rendering all vertices and indices
		RenderObject m_renderObject;

		// Underlying 1D texture used for passing the colors of all shapes to the shader
		Graphics::Texture1D m_texture;

		// Flag indicating if shapes batch is valid, meaning that it has been created and not yet destroyed
		bool m_isValid = false;
	};

} // namespace Renderer
} // namespace Pekan