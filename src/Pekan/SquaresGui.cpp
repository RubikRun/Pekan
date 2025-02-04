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

    static void renderSliderX(Rectangle& square, int windowWidth)
    {
        ImGui::PushItemWidth(0.4f * ImGui::GetContentRegionAvail().x);
        ImGui::Text("X");
        ImGui::SameLine();
        ImGui::SliderInt("##X", &square.x, 0, windowWidth);
        ImGui::PopItemWidth();
    }

    static void renderSliderY(Rectangle& square, int windowHeight)
    {
        ImGui::PushItemWidth(0.45f * ImGui::GetContentRegionAvail().x);
        ImGui::SameLine();
        ImGui::Text("Y");
        ImGui::SameLine();
        ImGui::SliderInt("##Y", &square.y, 0, windowHeight);
        ImGui::PopItemWidth();
    }

    static void renderSliderSize(Rectangle& square, int windowWidth, int windowHeight)
    {
        ImGui::PushItemWidth(0.85f * ImGui::GetContentRegionAvail().x);
        ImGui::Text("Size");
        ImGui::SameLine();
        ImGui::SliderInt("##Size", &square.width, 0, std::max(windowWidth, windowHeight));
        square.height = square.width;
        ImGui::PopItemWidth();
    }

    static void renderSliderRotation(Rectangle& square)
    {
        ImGui::PushItemWidth(0.75f * ImGui::GetContentRegionAvail().x);
        ImGui::Text("Rotation");
        ImGui::SameLine();
        ImGui::SliderInt("##Rotation", &square.rotation, 0, 360);
        ImGui::PopItemWidth();
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
        for (Rectangle& square : squares)
        {
            ImGui::PushID(square.id);

            ImGui::Text("Square %d", square.id + 1);
            renderSliderX(square, squaresScene.getWindowWidth());
            renderSliderY(square, squaresScene.getWindowHeight());
            renderSliderSize(square, squaresScene.getWindowWidth(), squaresScene.getWindowHeight());
            renderSliderRotation(square);
            ImGui::Separator();

            ImGui::PopID(); // End unique ID scope
        }

        ImGui::End();
    }
} // namespace Pekan