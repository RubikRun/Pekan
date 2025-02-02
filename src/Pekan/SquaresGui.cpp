#include "SquaresGui.h"

#include "SquaresScene.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>

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
        ImGui::SetNextWindowSize(ImVec2(300, 720));
        ImGui::Begin("Squares");

        if (ImGui::Button("+"))
        {
            squaresScene.addSquare();
        }

        std::vector<Rectangle>& squares = squaresScene.getSquares();
        for (auto& square : squares)
        {
            ImGui::PushID(square.id);

            ImGui::Text("Square %d", square.id + 1);
            ImGui::SliderInt("X", &square.x, 0, squaresScene.getWindowWidth());
            ImGui::SliderInt("Y", &square.y, 0, squaresScene.getWindowHeight());
            ImGui::SliderInt("Size", &square.width, 0, std::max(squaresScene.getWindowWidth(), squaresScene.getWindowHeight()));
            square.height = square.width;
            ImGui::Separator();

            ImGui::PopID(); // End unique ID scope
        }

        ImGui::End();
    }
} // namespace Pekan