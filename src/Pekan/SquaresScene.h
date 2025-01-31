#pragma once

namespace Pekan
{

	// A scene of multiple squares.
	// It begins empty and has API for adding new squares.
	// It also allows for moving, rotating, resizing and coloring each square.
	class SquaresScene
	{
	public:
		// Initializes the scene
		bool init();
		// Renders the scene
		void render();
		// Cleans up the scene
		void cleanup();

	private:
		// Shader program ID
		unsigned shaderProgram = 0;
		// Vertex array object ID
		unsigned vao = 0;
		// Vertex buffer object ID
		unsigned vbo = 0;
		// Element buffer object ID
		unsigned ebo = 0;
	};

} // namespace Pekan