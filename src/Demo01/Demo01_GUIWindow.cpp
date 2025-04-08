#include "Demo01_GUIWindow.h"
#include "Logger/PekanLogger.h"
#include "Demo01_Scene.h"
#include "PekanEngine.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using Pekan::PekanEngine;

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

    static void renderSliderSize(Rectangle& square, glm::vec2 resolution)
    {
        ImGui::PushItemWidth(0.85f * ImGui::GetContentRegionAvail().x);
        ImGui::Text("Size");
        ImGui::SameLine();
        ImGui::SliderInt("##Size", &square.width, 0, std::max(resolution.x, resolution.y));
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
        if (m_scene == nullptr)
        {
            PK_LOG_ERROR("Cannot render GUI window because there is no scene attached.", "Pekan");
            return;
        }

        ImGui::SetNextWindowSize(ImVec2(320, m_resolution.y));
        ImGui::Begin("Squares");

        if (ImGui::Button("+"))
        {
            m_scene->addSquare();
        }

        std::vector<Rectangle>& squares = m_scene->getSquares();
        for (Rectangle& square : squares)
        {
            ImGui::PushID(square.id);

            ImGui::Text("Square %d", square.id + 1);
            renderSliderX(square, m_resolution.x);
            renderSliderY(square, m_resolution.y);
            renderSliderSize(square, m_resolution);
            renderSliderRotation(square);
            renderEditColor(square);
            ImGui::Separator();

            ImGui::PopID(); // End unique ID scope
        }

        ImGui::End();
	}

	bool Demo01_GUIWindow::init()
	{
        m_resolution = PekanEngine::getWindowResolution();
        return true;
	}

} // namespace Demo