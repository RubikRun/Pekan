#include "Demo01_GUIWindow.h"
#define PK_FILENAME "Demo01_GUIWindow.cpp"
#include "Logger/PekanLogger.h"
#include "Demo01_Scene.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Demo
{

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

    static void renderEditColor(Rectangle& square)
    {
        ImGui::PushItemWidth(0.75f * ImGui::GetContentRegionAvail().x);
        ImGui::Text("Color");
        ImGui::SameLine();
        ImGui::ColorEdit4("##Color", (float*)&square.color);
        ImGui::PopItemWidth();
    }

	void Demo01_GUIWindow::_render()
	{
        if (scene == nullptr)
        {
            PK_LOG_ERRORF("Cannot render GUI window because there is no scene attached.");
            return;
        }

        ImGui::SetNextWindowSize(ImVec2(300, 720));
        ImGui::Begin("Squares");

        if (ImGui::Button("+"))
        {
            scene->addSquare();
        }

        std::vector<Rectangle>& squares = scene->getSquares();
        for (Rectangle& square : squares)
        {
            ImGui::PushID(square.id);

            ImGui::Text("Square %d", square.id + 1);
            renderSliderX(square, scene->getWidth());
            renderSliderY(square, scene->getHeight());
            renderSliderSize(square, scene->getWidth(), scene->getHeight());
            renderSliderRotation(square);
            renderEditColor(square);
            ImGui::Separator();

            ImGui::PopID(); // End unique ID scope
        }

        ImGui::End();
	}

	bool Demo01_GUIWindow::_init()
	{
		return true;
	}

} // namespace Demo