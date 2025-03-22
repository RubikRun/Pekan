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

	private: /* functions */

		void move();

		void moveSquare(int idx);

	private: /* variables */

		Pekan::Renderer::VertexArray m_vertexArray;

		Pekan::Renderer::VertexBuffer m_vertexBuffer;

		Pekan::Renderer::IndexBuffer m_indexBuffer;

		Pekan::Renderer::Shader m_shader;

		int m_squaresCount = -1;

		// Vertices of squares that make up snake's body
		std::vector<float> m_vertices;

		// Indices of vertices determined the order of drawing
		std::vector<int> m_indices;

		// Direction to which the snake is moving currently
		glm::ivec2 m_direction = { -1, 0 };

		// Number of frames rendered so far
		int m_frames = 0;
	};

} // namespace Demo