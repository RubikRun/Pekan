#include "Demo07_GUIWindow.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace Pekan;

const float CAMERA_SIZE = 10.0f;

namespace Demo
{

	static void renderSliderRectanglePosition(glm::vec2& rectanglePosition)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Position");
		ImGui::SliderFloat2("##RectanglePosition", &rectanglePosition.x, -CAMERA_SIZE * 2.0f, CAMERA_SIZE * 2.0f);
		ImGui::PopItemWidth();
	}

	void Demo07_GUIWindow::_render()
	{
		ImGui::SetNextWindowSize(ImVec2(300, 120));
		ImGui::Begin("Demo07");

		ImGui::Text("Rectangle");

		renderSliderRectanglePosition(m_rectanglePosition);

		ImGui::End();
	}

	bool Demo07_GUIWindow::init()
	{
		return true;
	}

} // namespace Demo