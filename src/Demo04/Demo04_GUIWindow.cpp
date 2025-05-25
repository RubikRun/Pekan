#include "Demo04_GUIWindow.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Demo
{

	static const char* SHADER_NAMES[] = { "Standard Blend", "Two Points Blend 1", "Two Points Blend 2", "Two Points Blend 3" };

	void Demo04_GUIWindow::_render()
	{
		ImGui::SetNextWindowSize(ImVec2(300, 210));
		ImGui::Begin("Pekan");

		ImGui::Text("Background Color");
		ImGui::ColorEdit3("##BackgroundColor", (float*)(&m_clearColor));

		ImGui::Text("Position");
		ImGui::DragFloat2("##Position", (float*)&(m_position), 0.01f, -1.0f, 1.0f);

		ImGui::Text("Enable Shapes");
		ImGui::Checkbox("##EnableShapes", &m_enabledShapes);

		ImGui::Text("Shader");
		ImGui::Combo("##Shader", &m_shaderIdx, SHADER_NAMES, IM_ARRAYSIZE(SHADER_NAMES));

		ImGui::End();
	}

	bool Demo04_GUIWindow::init()
	{
		return true;
	}

} // namespace Demo