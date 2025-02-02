#include "SquaresGui.h"

#include "SquaresScene.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Pekan
{
	void SquaresGui::render(GLFWwindow* window, SquaresScene& squaresScene)
	{
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            return;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderWindow(squaresScene);

        ImGui::Render();
	}
    void SquaresGui::renderWindow(SquaresScene& squaresScene)
    {
        ImGui::SetNextWindowSize(ImVec2(200, 70));
        ImGui::Begin("Squares");

        if (ImGui::Button("+")) {
            squaresScene.addRandomSquare();
        }

        ImGui::End();
    }
} // namespace Pekan