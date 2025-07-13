#include "Demo05_GUIWindow.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Demo
{

	void Demo05_GUIWindow::_render()
	{
		ImGui::SetNextWindowSize(ImVec2(200, 120));
		ImGui::Begin("Pekan");

		ImGui::Text("Background Color");
		ImGui::ColorEdit3("##BackgroundColor", (float*)(&m_clearColor));

		ImGui::Text("Line Thickness");
		ImGui::SliderFloat("##LineThickness", &m_lineThickness, 0.00001f, 0.001f, "%.5f");

		ImGui::End();
	}

	bool Demo05_GUIWindow::init()
	{
		return true;
	}

} // namespace Demo