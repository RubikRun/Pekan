#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace Demo
{

	class Snake
	{
	public:

		bool init();

		void update();

		void render();

		void exit();

	private: /* variables */

		Pekan::Renderer::VertexArray m_vertexArray;

		Pekan::Renderer::VertexBuffer m_vertexBuffer;

		Pekan::Renderer::IndexBuffer m_indexBuffer;

		Pekan::Renderer::Shader m_shader;

		// Vertices of squares that make up snake's body
		std::vector<float> m_vertices;

		// Indices of vertices determined the order of drawing
		std::vector<int> m_indices;
	};

} // namespace Demo