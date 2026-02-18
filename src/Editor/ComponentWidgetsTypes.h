#pragma once

#include "TextWidget.h"
#include "SliderFloatWidget.h"
#include "SliderIntWidget.h"
#include "ColorEdit4Widget.h"
#include "CheckboxWidget.h"
#include "DragFloat2Widget.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
///// This file defines structs containing widgets for each component type supported in the editor
//////////////////////////////////////////////////////////////////////////////////////////////////

namespace Pekan
{
namespace GUI
{
	class GUIWindow;
}

namespace Editor
{
	struct Transform2DWidgets
	{
		Pekan::GUI::TextWidget_Ptr header = std::make_shared<Pekan::GUI::TextWidget>();
		Pekan::GUI::DragFloat2Widget_Ptr position = std::make_shared<Pekan::GUI::DragFloat2Widget>();
		Pekan::GUI::SliderFloatWidget_Ptr rotation = std::make_shared<Pekan::GUI::SliderFloatWidget>();
		Pekan::GUI::DragFloat2Widget_Ptr scale = std::make_shared<Pekan::GUI::DragFloat2Widget>();

		void create(Pekan::GUI::GUIWindow* guiWindow);
		void hide();
		void unhide();
	};

	struct SpriteWidgets
	{
		Pekan::GUI::TextWidget_Ptr header = std::make_shared<Pekan::GUI::TextWidget>();
		Pekan::GUI::SliderFloatWidget_Ptr width = std::make_shared<Pekan::GUI::SliderFloatWidget>();
		Pekan::GUI::SliderFloatWidget_Ptr height = std::make_shared<Pekan::GUI::SliderFloatWidget>();
		Pekan::GUI::DragFloat2Widget_Ptr texCoordMin = std::make_shared<Pekan::GUI::DragFloat2Widget>();
		Pekan::GUI::DragFloat2Widget_Ptr texCoordMax = std::make_shared<Pekan::GUI::DragFloat2Widget>();

		void create(Pekan::GUI::GUIWindow* guiWindow);
		void hide();
		void unhide();
	};

	struct RectangleGeometryWidgets
	{
		Pekan::GUI::TextWidget_Ptr header = std::make_shared<Pekan::GUI::TextWidget>();
		Pekan::GUI::SliderFloatWidget_Ptr width = std::make_shared<Pekan::GUI::SliderFloatWidget>();
		Pekan::GUI::SliderFloatWidget_Ptr height = std::make_shared<Pekan::GUI::SliderFloatWidget>();

		void create(Pekan::GUI::GUIWindow* guiWindow);
		void hide();
		void unhide();
	};

	struct CircleGeometryWidgets
	{
		Pekan::GUI::TextWidget_Ptr header = std::make_shared<Pekan::GUI::TextWidget>();
		Pekan::GUI::SliderFloatWidget_Ptr radius = std::make_shared<Pekan::GUI::SliderFloatWidget>();
		Pekan::GUI::SliderIntWidget_Ptr segmentsCount = std::make_shared<Pekan::GUI::SliderIntWidget>();

		void create(Pekan::GUI::GUIWindow* guiWindow);
		void hide();
		void unhide();
	};

	struct TriangleGeometryWidgets
	{
		Pekan::GUI::TextWidget_Ptr header = std::make_shared<Pekan::GUI::TextWidget>();
		Pekan::GUI::DragFloat2Widget_Ptr pointA = std::make_shared<Pekan::GUI::DragFloat2Widget>();
		Pekan::GUI::DragFloat2Widget_Ptr pointB = std::make_shared<Pekan::GUI::DragFloat2Widget>();
		Pekan::GUI::DragFloat2Widget_Ptr pointC = std::make_shared<Pekan::GUI::DragFloat2Widget>();

		void create(Pekan::GUI::GUIWindow* guiWindow);
		void hide();
		void unhide();
	};

	struct PolygonGeometryWidgets
	{
		Pekan::GUI::TextWidget_Ptr header = std::make_shared<Pekan::GUI::TextWidget>();
		Pekan::GUI::TextWidget_Ptr vertexCount = std::make_shared<Pekan::GUI::TextWidget>();

		void create(Pekan::GUI::GUIWindow* guiWindow);
		void hide();
		void unhide();
	};

	struct LineGeometryWidgets
	{
		Pekan::GUI::TextWidget_Ptr header = std::make_shared<Pekan::GUI::TextWidget>();
		Pekan::GUI::DragFloat2Widget_Ptr pointA = std::make_shared<Pekan::GUI::DragFloat2Widget>();
		Pekan::GUI::DragFloat2Widget_Ptr pointB = std::make_shared<Pekan::GUI::DragFloat2Widget>();
		Pekan::GUI::SliderFloatWidget_Ptr thickness = std::make_shared<Pekan::GUI::SliderFloatWidget>();

		void create(Pekan::GUI::GUIWindow* guiWindow);
		void hide();
		void unhide();
	};

	struct SolidColorMaterialWidgets
	{
		Pekan::GUI::TextWidget_Ptr header = std::make_shared<Pekan::GUI::TextWidget>();
		Pekan::GUI::ColorEdit4Widget_Ptr color = std::make_shared<Pekan::GUI::ColorEdit4Widget>();

		void create(Pekan::GUI::GUIWindow* guiWindow);
		void hide();
		void unhide();
	};

	struct LineWidgets
	{
		Pekan::GUI::TextWidget_Ptr header = std::make_shared<Pekan::GUI::TextWidget>();
		Pekan::GUI::DragFloat2Widget_Ptr pointA = std::make_shared<Pekan::GUI::DragFloat2Widget>();
		Pekan::GUI::DragFloat2Widget_Ptr pointB = std::make_shared<Pekan::GUI::DragFloat2Widget>();
		Pekan::GUI::ColorEdit4Widget_Ptr color = std::make_shared<Pekan::GUI::ColorEdit4Widget>();

		void create(Pekan::GUI::GUIWindow* guiWindow);
		void hide();
		void unhide();
	};

	struct Camera2DWidgets
	{
		Pekan::GUI::TextWidget_Ptr header = std::make_shared<Pekan::GUI::TextWidget>();
		Pekan::GUI::DragFloat2Widget_Ptr size = std::make_shared<Pekan::GUI::DragFloat2Widget>();
		Pekan::GUI::DragFloat2Widget_Ptr position = std::make_shared<Pekan::GUI::DragFloat2Widget>();
		Pekan::GUI::SliderFloatWidget_Ptr rotation = std::make_shared<Pekan::GUI::SliderFloatWidget>();
		Pekan::GUI::SliderFloatWidget_Ptr zoomLevel = std::make_shared<Pekan::GUI::SliderFloatWidget>();
		Pekan::GUI::CheckboxWidget_Ptr isPrimary = std::make_shared<Pekan::GUI::CheckboxWidget>();
		Pekan::GUI::CheckboxWidget_Ptr isControllable = std::make_shared<Pekan::GUI::CheckboxWidget>();

		void create(Pekan::GUI::GUIWindow* guiWindow);
		void hide();
		void unhide();
	};

} // namespace Editor
} // namespace Pekan