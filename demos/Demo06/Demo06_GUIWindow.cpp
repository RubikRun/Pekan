#include "Demo06_GUIWindow.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Demo
{

	void Demo06_GUIWindow::_render()
	{
		ImGui::SetNextWindowSize(ImVec2(300, 300));
		ImGui::Begin("Pekan");

		ImGui::Text("Number Of Shapes");
		ImGui::SliderInt("##NumberOfShapes", &m_numberOfShapes, 5, 60000);

		ImGui::Checkbox("Show Rectangles", &m_isEnabledRectangles);
		ImGui::Checkbox("Show Circles", &m_isEnabledCircles);
		ImGui::Checkbox("Show Circles Static", &m_isEnabledCirclesStatic);
		ImGui::Checkbox("Show Triangles", &m_isEnabledTriangles);
		ImGui::Checkbox("Show Polygons", &m_isEnabledPolygons);
		ImGui::Checkbox("Show Lines", &m_isEnabledLines);

		m_fpsWaitFrames--;
		if (m_fpsWaitFrames < 1)
		{
			m_fpsCache = ImGui::GetIO().Framerate;
			m_fpsWaitFrames = int(m_fpsCache);
		}

		// Display current FPS
		ImGui::Text("%.3f ms/frame", 1000.0f / m_fpsCache);
		ImGui::Text("%.1f FPS", m_fpsCache);

		ImGui::End();
	}

	bool Demo06_GUIWindow::init()
	{
		return true;
	}

} // namespace Demo