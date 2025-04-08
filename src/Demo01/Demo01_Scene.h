#pragma once

#include "PekanScene.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#include "Demo01_GUIWindow.h"

namespace Demo
{

	struct Rectangle {
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
		int rotation = 0;
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		int id = -1;
	};

	class Demo01_Scene : public Pekan::PekanScene
	{
	public:

		bool init() override;

		void update() override;

		void render() override;

		void exit() override;

		void addSquare();

		// Returns a (const) reference to the list of squares
		inline const std::vector<Rectangle>& getSquares() const { return m_squares; }
		inline std::vector<Rectangle>& getSquares() { return m_squares; }

	private: /* variables */

		// List holding currently existing squares
		std::vector<Rectangle> m_squares;
		// Array of vertex data of currently existing squares
		std::vector<float> m_vertices;

		Pekan::Renderer::VertexBuffer m_vertexBuffer;

		Pekan::Renderer::VertexArray m_vertexArray;

		Pekan::Renderer::Shader m_shader;

		glm::ivec2 m_resolution;
	};

} // namespace Demo