#pragma once

#include "GUIWindow.h"

#include "ComponentWidgetsTypes.h"

#include "TextWidget.h"
#include "ComboBoxWidget.h"
#include "SeparatorWidget.h"
#include "ButtonWidget.h"

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
		void setScene(std::shared_ptr<EditorScene> scene);

	private: /* functions */

		bool _init() override;

		Pekan::GUI::GUIWindowProperties getProperties() const override;

		// Hides all component widgets
		void hideComponentWidgets();

		void updateWidgetsFromComponentsOfEntity(entt::entity entity);
		void updateComponentsOfEntityFromWidgets(entt::entity entity);

		// Updates visibility of component widgets based on which components the given entity has
		void updateWidgetsVisibility(entt::entity entity);

	private: /* variables */

		struct Widgets
		{
			Pekan::GUI::TextWidget_Ptr entityInfoTextWidget = std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::SeparatorWidget_Ptr firstSeparatorWidget = std::make_shared<Pekan::GUI::SeparatorWidget>();
			Pekan::GUI::ComboBoxWidget_Ptr addComponentComboBoxWidget = std::make_shared<Pekan::GUI::ComboBoxWidget>();
			Pekan::GUI::ButtonWidget_Ptr addComponentButtonWidget = std::make_shared<Pekan::GUI::ButtonWidget>();
			Pekan::GUI::SeparatorWidget_Ptr secondSeparatorWidget = std::make_shared<Pekan::GUI::SeparatorWidget>();

			// Widgets for each component type supported in the editor
			Transform2DWidgets transform2DWidgets;
			SpriteWidgets spriteWidgets;
			RectangleGeometryWidgets rectangleGeometryWidgets;
			CircleGeometryWidgets circleGeometryWidgets;
			TriangleGeometryWidgets triangleGeometryWidgets;
			PolygonGeometryWidgets polygonGeometryWidgets;
			LineGeometryWidgets lineGeometryWidgets;
			SolidColorMaterialWidgets solidColorMaterialWidgets;
			LineWidgets lineWidgets;
			Camera2DWidgets camera2DWidgets;
		} gui;

		// Entity whose properties are currently displayed in this GUI window.
		entt::entity m_entity = entt::null;

		// Pointer to the scene to which the entity belongs
		std::shared_ptr<EditorScene> m_scene;
	};

} // namespace Editor
} // namespace Pekan
