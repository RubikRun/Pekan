#include "DemoGUIWindow.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Demo
{

	void DemoGUIWindow::_render()
	{
		ImGui::SetNextWindowSize(ImVec2(200, 110));
		ImGui::Begin("Pekan");

		ImGui::Text("Background Color");
		ImGui::ColorEdit3("", (float*)(&clearColor));

		ImGui::Text("Position");
		ImGui::DragFloat2("", (float*)&(position), 0.01f, -1.0f, 1.0f);

		ImGui::End();
	}

	bool DemoGUIWindow::_init()
	{
		return true;
	}

} // namespace Demo