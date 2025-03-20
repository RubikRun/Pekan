#include "Demo02_GUIWindow.h"
#include "Logger/PekanLogger.h"
#include "Demo02_Scene.h"
#include "PekanEngine.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm/glm.hpp"

using Pekan::PekanEngine;

namespace Demo
{
	static void renderEditColor(glm::vec3& color, int index)
	{
		const std::string colorLabel = std::string("Color ") + std::to_string(index);

		ImGui::PushItemWidth(0.75f * ImGui::GetContentRegionAvail().x);
		ImGui::Text(colorLabel.c_str());
		ImGui::SameLine();
		ImGui::ColorEdit3((std::string("##") + colorLabel).c_str(), (float*)&color);
		ImGui::PopItemWidth();
	}

	static void renderSliderRotation(float& rotation)
	{
		ImGui::PushItemWidth(0.75f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::SliderFloat("##Rotation", &rotation, 0.0f, 360.0f);
		ImGui::PopItemWidth();
	}

	static void renderSliderFov(float& fov)
	{
		ImGui::PushItemWidth(0.63f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Field Of View");
		ImGui::SameLine();
		ImGui::SliderFloat("##FOV", &fov, 10.0f, 100.0f);
		ImGui::PopItemWidth();
	}

	void Demo02_GUIWindow::_render()
	{
		if (m_scene == nullptr)
		{
			PK_LOG_ERROR("Cannot render GUI window because there is no scene attached.", "Pekan");
			return;
		}

		ImGui::SetNextWindowSize(ImVec2(320, PekanEngine::getWindowHeight()));
		ImGui::Begin("Cube");

		std::vector<glm::vec3>& colors = m_scene->getColors();
		for (int i = 0; i < colors.size(); i++)
		{
			ImGui::PushID(i);
			renderEditColor(colors[i], i);
			ImGui::PopID();
		}
		ImGui::Separator();

		renderSliderRotation(m_scene->getRotation());
		renderSliderFov(m_scene->getFOV());

		ImGui::End();
	}

	bool Demo02_GUIWindow::init()
	{
		return true;
	}

} // namespace Demo