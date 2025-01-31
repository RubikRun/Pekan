#pragma once

struct GLFWwindow;
struct ImVec4;

namespace Pekan
{
	// A window containing GUI controls for SquaresScene
	class SquaresGui
	{
	public:
		// Renders the GUI
		static void render(GLFWwindow* window, ImVec4& clearColor);

	private:

		// Renders the GUI window only
		static void renderWindow(ImVec4& clearColor);
	};

} // namespace Pekan