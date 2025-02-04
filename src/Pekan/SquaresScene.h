#pragma once

#include <vector>

namespace Pekan
{
	struct Rectangle {
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
		int rotation = 0;
		int id = -1;
	};

	// A scene of multiple squares.
	// It begins empty and has API for adding new squares.
	// It also allows for moving, rotating, resizing and coloring each square.
	class SquaresScene
	{
	public:
		// Initializes the scene
		bool init(int windowWidth, int windowHeight);
		// Updates the scene
		void update();
		// Renders the scene
		void render();
		// Cleans up the scene
		void cleanup();

		void addSquare();

		// Returns a (const) reference to the list of squares
		const std::vector<Rectangle>& getSquares() const { return squares; }
		std::vector<Rectangle>& getSquares() { return squares; }

		int getWindowWidth() const { return windowWidth; }
		int getWindowHeight() const { return windowHeight; }

	private:
		// Shader program ID
		unsigned shaderProgram = 0;
		// Vertex array object ID
		unsigned vao = 0;
		// Vertex buffer object ID
		unsigned vbo = 0;

		// List holding currently existing squares
		std::vector<Rectangle> squares;
		// Array of vertex data of currently existing squares
		std::vector<float> vertices;

		int windowWidth = -1;
		int windowHeight = -1;
	};

} // namespace Pekan