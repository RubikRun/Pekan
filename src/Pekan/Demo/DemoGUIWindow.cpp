#include "DemoGUIWindow.h"
#define PK_FILENAME "DemoGUIWindow.cpp"
#include "Logger/PekanLogger.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{

	void DemoGUIWindow::_render()
	{
		ImGui::SetNextWindowSize(ImVec2(200, 70));
		ImGui::Begin("Pekan");

		ImGui::Text("Background Color");
		ImGui::ColorEdit3("", (float*)&clearColor);

		ImGui::End();
	}

	bool DemoGUIWindow::_init()
	{
		return true;
	}

} // namespace Pekan