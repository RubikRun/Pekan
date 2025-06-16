#include "Demo07_GUIWindow.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Demo
{

	void Demo07_GUIWindow::_render()
	{
		ImGui::SetNextWindowSize(ImVec2(200, 120));
		ImGui::Begin("Demo07");

		ImGui::End();
	}

	bool Demo07_GUIWindow::init()
	{
		return true;
	}

} // namespace Demo