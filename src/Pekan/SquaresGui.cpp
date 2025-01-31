#include "SquaresGui.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Pekan
{
	void SquaresGui::render(GLFWwindow* window, ImVec4& clearColor)
	{
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            return;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderWindow(clearColor);

        ImGui::Render();
	}
    void SquaresGui::renderWindow(ImVec4& clearColor)
    {
        ImGui::SetNextWindowSize(ImVec2(200, 70));
        ImGui::Begin("Pekan");

        ImGui::Text("Background Color");
        ImGui::ColorEdit3("", (float*)&clearColor);

        ImGui::End();
    }
} // namespace Pekan