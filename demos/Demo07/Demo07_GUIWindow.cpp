#include "Demo07_GUIWindow.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define PI 3.14159265359f

using namespace Pekan;

const float CAMERA_SIZE = 10.0f;

namespace Demo
{
	// Rectangle sliders

	static void renderSliderRectanglePosition(glm::vec2& rectanglePosition)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Position");
		ImGui::SliderFloat2("##RectanglePosition", &rectanglePosition.x, -CAMERA_SIZE, CAMERA_SIZE);
		ImGui::PopItemWidth();
	}
	static void renderSliderRectangleRotation(float& rectangleRotation)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Rotation");
		ImGui::SliderFloat("##RectangleRotation", &rectangleRotation, -PI, PI * 2.0f);
		ImGui::PopItemWidth();
	}
	static void renderSliderRectangleScale(glm::vec2& rectangleScale)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Scale");
		ImGui::SliderFloat2("##RectangleScale", &rectangleScale.x, -3.0f, 3.0f);
		ImGui::PopItemWidth();
	}

	// Circle sliders

	static void renderSliderCirclePosition(glm::vec2& circlePosition)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Position");
		ImGui::SliderFloat2("##CirclePosition", &circlePosition.x, -CAMERA_SIZE, CAMERA_SIZE);
		ImGui::PopItemWidth();
	}
	static void renderSliderCircleRotation(float& circleRotation)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Rotation");
		ImGui::SliderFloat("##CircleRotation", &circleRotation, -PI, PI * 2.0f);
		ImGui::PopItemWidth();
	}
	static void renderSliderCircleScale(glm::vec2& circleScale)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Scale");
		ImGui::SliderFloat2("##CircleScale", &circleScale.x, -3.0f, 3.0f);
		ImGui::PopItemWidth();
	}

	// CircleStatic sliders

	static void renderSliderCircleStaticPosition(glm::vec2& circleStaticPosition)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Position");
		ImGui::SliderFloat2("##CircleStaticPosition", &circleStaticPosition.x, -CAMERA_SIZE, CAMERA_SIZE);
		ImGui::PopItemWidth();
	}
	static void renderSliderCircleStaticRotation(float& circleStaticRotation)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Rotation");
		ImGui::SliderFloat("##CircleStaticRotation", &circleStaticRotation, -PI, PI * 2.0f);
		ImGui::PopItemWidth();
	}
	static void renderSliderCircleStaticScale(glm::vec2& circleStaticScale)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Scale");
		ImGui::SliderFloat2("##CircleStaticScale", &circleStaticScale.x, -3.0f, 3.0f);
		ImGui::PopItemWidth();
	}

	// Triangle sliders

	static void renderSliderTrianglePosition(glm::vec2& trianglePosition)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Position");
		ImGui::SliderFloat2("##TrianglePosition", &trianglePosition.x, -CAMERA_SIZE, CAMERA_SIZE);
		ImGui::PopItemWidth();
	}
	static void renderSliderTriangleRotation(float& triangleRotation)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Rotation");
		ImGui::SliderFloat("##TriangleRotation", &triangleRotation, -PI, PI * 2.0f);
		ImGui::PopItemWidth();
	}
	static void renderSliderTriangleScale(glm::vec2& triangleScale)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Scale");
		ImGui::SliderFloat2("##TriangleScale", &triangleScale.x, -3.0f, 3.0f);
		ImGui::PopItemWidth();
	}

	// Polygon sliders

	static void renderSliderPolygonPosition(glm::vec2& polygonPosition)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Position");
		ImGui::SliderFloat2("##PolygonPosition", &polygonPosition.x, -CAMERA_SIZE, CAMERA_SIZE);
		ImGui::PopItemWidth();
	}
	static void renderSliderPolygonRotation(float& polygonRotation)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Rotation");
		ImGui::SliderFloat("##PolygonRotation", &polygonRotation, -PI, PI * 2.0f);
		ImGui::PopItemWidth();
	}
	static void renderSliderPolygonScale(glm::vec2& polygonScale)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Scale");
		ImGui::SliderFloat2("##PolygonScale", &polygonScale.x, -3.0f, 3.0f);
		ImGui::PopItemWidth();
	}

	// Line sliders

	static void renderSliderLinePosition(glm::vec2& linePosition)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Position");
		ImGui::SliderFloat2("##LinePosition", &linePosition.x, -CAMERA_SIZE, CAMERA_SIZE);
		ImGui::PopItemWidth();
	}
	static void renderSliderLineRotation(float& lineRotation)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Rotation");
		ImGui::SliderFloat("##LineRotation", &lineRotation, -PI, PI * 2.0f);
		ImGui::PopItemWidth();
	}
	static void renderSliderLineScale(glm::vec2& lineScale)
	{
		ImGui::PushItemWidth(0.95f * ImGui::GetContentRegionAvail().x);
		ImGui::Text("Scale");
		ImGui::SliderFloat2("##LineScale", &lineScale.x, -3.0f, 3.0f);
		ImGui::PopItemWidth();
	}

	void Demo07_GUIWindow::_render()
	{
		ImGui::SetNextWindowSize(ImVec2(300, 1020));
		ImGui::Begin("Demo07");

		ImGui::Text("Rectangle");
		ImGui::Separator();
		renderSliderRectanglePosition(m_rectanglePosition);
		renderSliderRectangleRotation(m_rectangleRotation);
		renderSliderRectangleScale(m_rectangleScale);

		ImGui::NewLine();

		ImGui::Text("Circle");
		ImGui::Separator();
		renderSliderCirclePosition(m_circlePosition);
		renderSliderCircleRotation(m_circleRotation);
		renderSliderCircleScale(m_circleScale);

		ImGui::NewLine();

		ImGui::Text("CircleStatic");
		ImGui::Separator();
		renderSliderCircleStaticPosition(m_circleStaticPosition);
		renderSliderCircleStaticRotation(m_circleStaticRotation);
		renderSliderCircleStaticScale(m_circleStaticScale);

		ImGui::NewLine();

		ImGui::Text("Triangle");
		ImGui::Separator();
		renderSliderTrianglePosition(m_trianglePosition);
		renderSliderTriangleRotation(m_triangleRotation);
		renderSliderTriangleScale(m_triangleScale);

		ImGui::NewLine();

		ImGui::Text("Polygon");
		ImGui::Separator();
		renderSliderPolygonPosition(m_polygonPosition);
		renderSliderPolygonRotation(m_polygonRotation);
		renderSliderPolygonScale(m_polygonScale);

		ImGui::NewLine();

		ImGui::Text("Line");
		ImGui::Separator();
		renderSliderLinePosition(m_linePosition);
		renderSliderLineRotation(m_lineRotation);
		renderSliderLineScale(m_lineScale);

		ImGui::End();
	}

	bool Demo07_GUIWindow::init()
	{
		return true;
	}

} // namespace Demo