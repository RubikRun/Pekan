#include "Demo04_GUIWindow.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Demo
{

	void Demo04_GUIWindow::_render()
	{
		ImGui::SetNextWindowSize(ImVec2(200, 160));
		ImGui::Begin("Pekan");

		ImGui::Text("Background Color");
		ImGui::ColorEdit3("", (float*)(&m_clearColor));

		ImGui::Text("Position");
		ImGui::DragFloat2("", (float*)&(m_position), 0.01f, -1.0f, 1.0f);

		ImGui::Text("Enable Shapes");
		ImGui::Checkbox("##", &m_enabledShapes);

		ImGui::End();
	}

	bool Demo04_GUIWindow::init()
	{
		return true;
	}

} // namespace Demo