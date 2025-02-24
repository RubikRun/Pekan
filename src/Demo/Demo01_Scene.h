#pragma once

#include "PekanScene.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
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

		void update() override;

		void render() override;

		void exit() override;
		
		inline int getWidth() const { return width; }
		inline int getHeight() const { return height; }

		void addSquare();

		// Returns a (const) reference to the list of squares
		const std::vector<Rectangle>& getSquares() const { return squares; }
		std::vector<Rectangle>& getSquares() { return squares; }

	private: /* functions */

		bool _init() override;

	private: /* variables */

		// List holding currently existing squares
		std::vector<Rectangle> squares;
		// Array of vertex data of currently existing squares
		std::vector<float> vertices;

		Pekan::Renderer::Shader shader;

		unsigned vao = 0;
		unsigned vbo = 0;
	};

} // namespace Demo