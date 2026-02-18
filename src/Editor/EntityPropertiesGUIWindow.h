#pragma once

#include "GUIWindow.h"

#include "TextWidget.h"
#include "ComboBoxWidget.h"
#include "SeparatorWidget.h"
#include "ButtonWidget.h"
#include "SliderFloatWidget.h"
#include "SliderIntWidget.h"
#include "ColorEdit4Widget.h"
#include "CheckboxWidget.h"
#include "DragFloat2Widget.h"

#include <entt/entt.hpp>

namespace Pekan
{
namespace Editor
{

	class EditorScene;

	class EntityPropertiesGUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

		EntityPropertiesGUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		std::string getLayerName() const override { return "entity_properties_gui_window"; }

		void update(double deltaTime) override;

		// Sets the entity whose properties will be displayed in this GUI window
		void setEntity(entt::entity entity);

		// Sets the scene to which the entity belongs
		void setScene(std::shared_ptr<EditorScene> scene) { m_scene = scene; }

	private: /* functions */

		bool _init() override;

		Pekan::GUI::GUIWindowProperties getProperties() const override;

		// Syncs component data from entity to widgets (call when entity changes)
		void syncComponentToWidgets(entt::entity entity);
		// Syncs widget data to entity's components (call every frame for visible components)
		void syncWidgetsToComponents(entt::entity entity);
		// Updates visibility of component widgets based on entity's components
		void updateComponentWidgetsVisibility(entt::entity entity);

	private: /* variables */

		struct Widgets
		{
			Pekan::GUI::TextWidget_Ptr entityInfoTextWidget = std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::SeparatorWidget_Ptr separatorWidget = std::make_shared<Pekan::GUI::SeparatorWidget>();
			Pekan::GUI::ComboBoxWidget_Ptr addComponentComboBoxWidget = std::make_shared<Pekan::GUI::ComboBoxWidget>();
			Pekan::GUI::ButtonWidget_Ptr addComponentButtonWidget = std::make_shared<Pekan::GUI::ButtonWidget>();
			Pekan::GUI::SeparatorWidget_Ptr componentsSeparatorWidget = std::make_shared<Pekan::GUI::SeparatorWidget>();

			// Transform2D
			Pekan::GUI::TextWidget_Ptr transform2DHeader = std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::DragFloat2Widget_Ptr transform2DPosition = std::make_shared<Pekan::GUI::DragFloat2Widget>();
			Pekan::GUI::SliderFloatWidget_Ptr transform2DRotation = std::make_shared<Pekan::GUI::SliderFloatWidget>();
			Pekan::GUI::DragFloat2Widget_Ptr transform2DScale = std::make_shared<Pekan::GUI::DragFloat2Widget>();

			// Sprite
			Pekan::GUI::TextWidget_Ptr spriteHeader = std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::SliderFloatWidget_Ptr spriteWidth = std::make_shared<Pekan::GUI::SliderFloatWidget>();
			Pekan::GUI::SliderFloatWidget_Ptr spriteHeight = std::make_shared<Pekan::GUI::SliderFloatWidget>();
			Pekan::GUI::DragFloat2Widget_Ptr spriteTexCoordMin = std::make_shared<Pekan::GUI::DragFloat2Widget>();
			Pekan::GUI::DragFloat2Widget_Ptr spriteTexCoordMax = std::make_shared<Pekan::GUI::DragFloat2Widget>();

			// Camera2D
			Pekan::GUI::TextWidget_Ptr camera2DHeader = std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::DragFloat2Widget_Ptr camera2DSize = std::make_shared<Pekan::GUI::DragFloat2Widget>();
			Pekan::GUI::DragFloat2Widget_Ptr camera2DPosition = std::make_shared<Pekan::GUI::DragFloat2Widget>();
			Pekan::GUI::SliderFloatWidget_Ptr camera2DRotation = std::make_shared<Pekan::GUI::SliderFloatWidget>();
			Pekan::GUI::SliderFloatWidget_Ptr camera2DZoomLevel = std::make_shared<Pekan::GUI::SliderFloatWidget>();
			Pekan::GUI::CheckboxWidget_Ptr camera2DIsPrimary = std::make_shared<Pekan::GUI::CheckboxWidget>();
			Pekan::GUI::CheckboxWidget_Ptr camera2DIsControllable = std::make_shared<Pekan::GUI::CheckboxWidget>();

			// Line
			Pekan::GUI::TextWidget_Ptr lineHeader = std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::DragFloat2Widget_Ptr linePointA = std::make_shared<Pekan::GUI::DragFloat2Widget>();
			Pekan::GUI::DragFloat2Widget_Ptr linePointB = std::make_shared<Pekan::GUI::DragFloat2Widget>();
			Pekan::GUI::ColorEdit4Widget_Ptr lineColor = std::make_shared<Pekan::GUI::ColorEdit4Widget>();

			// RectangleGeometry
			Pekan::GUI::TextWidget_Ptr rectangleGeometryHeader = std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::SliderFloatWidget_Ptr rectangleGeometryWidth = std::make_shared<Pekan::GUI::SliderFloatWidget>();
			Pekan::GUI::SliderFloatWidget_Ptr rectangleGeometryHeight = std::make_shared<Pekan::GUI::SliderFloatWidget>();

			// CircleGeometry
			Pekan::GUI::TextWidget_Ptr circleGeometryHeader = std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::SliderFloatWidget_Ptr circleGeometryRadius = std::make_shared<Pekan::GUI::SliderFloatWidget>();
			Pekan::GUI::SliderIntWidget_Ptr circleGeometrySegmentsCount = std::make_shared<Pekan::GUI::SliderIntWidget>();

			// TriangleGeometry
			Pekan::GUI::TextWidget_Ptr triangleGeometryHeader = std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::DragFloat2Widget_Ptr triangleGeometryPointA = std::make_shared<Pekan::GUI::DragFloat2Widget>();
			Pekan::GUI::DragFloat2Widget_Ptr triangleGeometryPointB = std::make_shared<Pekan::GUI::DragFloat2Widget>();
			Pekan::GUI::DragFloat2Widget_Ptr triangleGeometryPointC = std::make_shared<Pekan::GUI::DragFloat2Widget>();

			// PolygonGeometry (vertex count display only for now)
			Pekan::GUI::TextWidget_Ptr polygonGeometryHeader = std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::TextWidget_Ptr polygonGeometryVertexCount = std::make_shared<Pekan::GUI::TextWidget>();

			// LineGeometry
			Pekan::GUI::TextWidget_Ptr lineGeometryHeader = std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::DragFloat2Widget_Ptr lineGeometryPointA = std::make_shared<Pekan::GUI::DragFloat2Widget>();
			Pekan::GUI::DragFloat2Widget_Ptr lineGeometryPointB = std::make_shared<Pekan::GUI::DragFloat2Widget>();
			Pekan::GUI::SliderFloatWidget_Ptr lineGeometryThickness = std::make_shared<Pekan::GUI::SliderFloatWidget>();

			// SolidColorMaterial
			Pekan::GUI::TextWidget_Ptr solidColorMaterialHeader = std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::ColorEdit4Widget_Ptr solidColorMaterialColor = std::make_shared<Pekan::GUI::ColorEdit4Widget>();
		} gui;

		// Entity whose properties are currently displayed in this GUI window.
		entt::entity m_entity = entt::null;

		// Pointer to the scene to which the entity belongs
		std::shared_ptr<EditorScene> m_scene;
	};

} // namespace Editor
} // namespace Pekan
