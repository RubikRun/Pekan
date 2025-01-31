#pragma once

struct GLFWwindow;
struct ImVec4;

namespace Pekan
{
	// A window containing GUI controls for SquaresScene
	class SquaresGui
	{
	public:
		// Renders the GUI window
		static void render(GLFWwindow* window, ImVec4& clearColor);
	};

} // namespace Pekan