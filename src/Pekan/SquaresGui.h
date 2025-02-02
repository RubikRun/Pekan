#pragma once

struct GLFWwindow;
struct ImVec4;

namespace Pekan
{
	class SquaresScene;

	// A window containing GUI controls for SquaresScene
	class SquaresGui
	{
	public:
		// Renders the GUI
		static void render(GLFWwindow* window, SquaresScene& squaresScene);

	private:

		// Renders the GUI window only
		static void renderWindow(SquaresScene& squaresScene);
	};

} // namespace Pekan