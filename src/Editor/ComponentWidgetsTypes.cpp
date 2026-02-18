#include "ComponentWidgetsTypes.h"

namespace Pekan
{
namespace Editor
{

	static constexpr float PI = 3.14159265358979323846f;

	void Transform2DWidgets::create(Pekan::GUI::GUIWindow* guiWindow)
	{
		header->create(guiWindow, "Transform2D");
		position->create(guiWindow, "Position", glm::vec2(0.0f), -1000.0f, 1000.0f, 0.01f, "%.3f");
		rotation->create(guiWindow, "Rotation", 0.0f, -PI, PI, "%.3f");
		scale->create(guiWindow, "Scale", glm::vec2(1.0f), 0.01f, 100.0f, 0.01f, "%.3f");
	}

	void Transform2DWidgets::hide()
	{
		header->hide();
		position->hide();
		rotation->hide();
		scale->hide();
	}

	void Transform2DWidgets::unhide()
	{
		header->unhide();
		position->unhide();
		rotation->unhide();
		scale->unhide();
	}

	void SpriteWidgets::create(Pekan::GUI::GUIWindow* guiWindow)
	{
		header->create(guiWindow, "Sprite");
		width->create(guiWindow, "Width", 1.0f, 0.01f, 100.0f, "%.3f");
		height->create(guiWindow, "Height", 1.0f, 0.01f, 100.0f, "%.3f");
		texCoordMin->create(guiWindow, "Tex Coord Min", glm::vec2(0.0f), 0.0f, 1.0f, 0.01f, "%.3f");
		texCoordMax->create(guiWindow, "Tex Coord Max", glm::vec2(1.0f), 0.0f, 1.0f, 0.01f, "%.3f");
	}

	void SpriteWidgets::hide()
	{
		header->hide();
		width->hide();
		height->hide();
		texCoordMin->hide();
		texCoordMax->hide();
	}

	void SpriteWidgets::unhide()
	{
		header->unhide();
		width->unhide();
		height->unhide();
		texCoordMin->unhide();
		texCoordMax->unhide();
	}

	void RectangleGeometryWidgets::create(Pekan::GUI::GUIWindow* guiWindow)
	{
		header->create(guiWindow, "RectangleGeometry");
		width->create(guiWindow, "Width", 1.0f, 0.01f, 100.0f, "%.3f");
		height->create(guiWindow, "Height", 1.0f, 0.01f, 100.0f, "%.3f");
	}

	void RectangleGeometryWidgets::hide()
	{
		header->hide();
		width->hide();
		height->hide();
	}

	void RectangleGeometryWidgets::unhide()
	{
		header->unhide();
		width->unhide();
		height->unhide();
	}

	void CircleGeometryWidgets::create(Pekan::GUI::GUIWindow* guiWindow)
	{
		header->create(guiWindow, "CircleGeometry");
		radius->create(guiWindow, "Radius", 1.0f, 0.01f, 100.0f, "%.3f");
		segmentsCount->create(guiWindow, "Segments", 32, 3, 128);
	}

	void CircleGeometryWidgets::hide()
	{
		header->hide();
		radius->hide();
		segmentsCount->hide();
	}

	void CircleGeometryWidgets::unhide()
	{
		header->unhide();
		radius->unhide();
		segmentsCount->unhide();
	}

	void TriangleGeometryWidgets::create(Pekan::GUI::GUIWindow* guiWindow)
	{
		header->create(guiWindow, "TriangleGeometry");
		pointA->create(guiWindow, "Point A", glm::vec2(-1.0f, 0.0f), -1000.0f, 1000.0f, 0.01f, "%.3f");
		pointB->create(guiWindow, "Point B", glm::vec2(1.0f, 0.0f), -1000.0f, 1000.0f, 0.01f, "%.3f");
		pointC->create(guiWindow, "Point C", glm::vec2(0.0f, 1.0f), -1000.0f, 1000.0f, 0.01f, "%.3f");
	}

	void TriangleGeometryWidgets::hide()
	{
		header->hide();
		pointA->hide();
		pointB->hide();
		pointC->hide();
	}

	void TriangleGeometryWidgets::unhide()
	{
		header->unhide();
		pointA->unhide();
		pointB->unhide();
		pointC->unhide();
	}

	void PolygonGeometryWidgets::create(Pekan::GUI::GUIWindow* guiWindow)
	{
		header->create(guiWindow, "PolygonGeometry");
		vertexCount->create(guiWindow, "Vertices: 0");
	}

	void PolygonGeometryWidgets::hide()
	{
		header->hide();
		vertexCount->hide();
	}

	void PolygonGeometryWidgets::unhide()
	{
		header->unhide();
		vertexCount->unhide();
	}

	void LineGeometryWidgets::create(Pekan::GUI::GUIWindow* guiWindow)
	{
		header->create(guiWindow, "LineGeometry");
		pointA->create(guiWindow, "Point A", glm::vec2(0.0f), -1000.0f, 1000.0f, 0.01f, "%.3f");
		pointB->create(guiWindow, "Point B", glm::vec2(1.0f), -1000.0f, 1000.0f, 0.01f, "%.3f");
		thickness->create(guiWindow, "Thickness", 0.02f, 0.001f, 10.0f, "%.3f");
	}

	void LineGeometryWidgets::hide()
	{
		header->hide();
		pointA->hide();
		pointB->hide();
		thickness->hide();
	}

	void LineGeometryWidgets::unhide()
	{
		header->unhide();
		pointA->unhide();
		pointB->unhide();
		thickness->unhide();
	}

	void SolidColorMaterialWidgets::create(Pekan::GUI::GUIWindow* guiWindow)
	{
		header->create(guiWindow, "SolidColorMaterial");
		color->create(guiWindow, "Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	void SolidColorMaterialWidgets::hide()
	{
		header->hide();
		color->hide();
	}

	void SolidColorMaterialWidgets::unhide()
	{
		header->unhide();
		color->unhide();
	}

	void LineWidgets::create(Pekan::GUI::GUIWindow* guiWindow)
	{
		header->create(guiWindow, "Line");
		pointA->create(guiWindow, "Point A", glm::vec2(0.0f), -1000.0f, 1000.0f, 0.01f, "%.3f");
		pointB->create(guiWindow, "Point B", glm::vec2(1.0f), -1000.0f, 1000.0f, 0.01f, "%.3f");
		color->create(guiWindow, "Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	void LineWidgets::hide()
	{
		header->hide();
		pointA->hide();
		pointB->hide();
		color->hide();
	}

	void LineWidgets::unhide()
	{
		header->unhide();
		pointA->unhide();
		pointB->unhide();
		color->unhide();
	}

	void Camera2DWidgets::create(Pekan::GUI::GUIWindow* guiWindow)
	{
		header->create(guiWindow, "Camera2D");
		size->create(guiWindow, "Size", glm::vec2(16.0f, 9.0f), 0.1f, 1000.0f, 0.1f, "%.2f");
		position->create(guiWindow, "Position", glm::vec2(0.0f), -1000.0f, 1000.0f, 0.01f, "%.3f");
		rotation->create(guiWindow, "Rotation", 0.0f, -PI, PI, "%.3f");
		zoomLevel->create(guiWindow, "Zoom", 1.0f, 0.01f, 10.0f, "%.2f");
		isPrimary->create(guiWindow, "Primary", true);
		isControllable->create(guiWindow, "Controllable", true);
	}

	void Camera2DWidgets::hide()
	{
		header->hide();
		size->hide();
		position->hide();
		rotation->hide();
		zoomLevel->hide();
		isPrimary->hide();
		isControllable->hide();
	}

	void Camera2DWidgets::unhide()
	{
		header->unhide();
		size->unhide();
		position->unhide();
		rotation->unhide();
		zoomLevel->unhide();
		isPrimary->unhide();
		isControllable->unhide();
	}

} // namespace Editor
} // namespace Pekan
