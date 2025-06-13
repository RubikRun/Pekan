#include "Demo06_GUIWindow.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Demo
{

	void Demo06_GUIWindow::_render()
	{
		ImGui::SetNextWindowSize(ImVec2(300, 80));
		ImGui::Begin("Pekan");

		ImGui::Text("Number Of Shapes");
		ImGui::SliderInt("##NumberOfShapes", &m_numberOfShapes, 5, 5000);

		ImGui::End();
	}

	bool Demo06_GUIWindow::init()
	{
		return true;
	}

} // namespace Demo