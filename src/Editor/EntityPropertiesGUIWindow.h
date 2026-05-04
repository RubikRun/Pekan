#pragma once

#include "GUIWindow.h"

#include "ComponentWidgetsTypes.h"

#include "TextWidget.h"
#include "CheckboxWidget.h"
#include "SeparatorWidget.h"
#include "ContextMenuWidget.h"

#include "Entity/EntityID.h"

#include <entt/entt.hpp>

namespace Pekan
{
namespace Editor
{

	class EditorScene;

	class EntityPropertiesGUIWindow : public Pekan::GUI::GUIWindow
	{
		bool _init() override;

		void update(double deltaTime) override;

	public:

		EntityPropertiesGUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		std::string getLayerName() const override { return "entity_properties_gui_window"; }

		// Sets the entity whose properties will be displayed in this GUI window.
		// Pass INVALID_ENTITY_ID to clear the selection.
		void setEntity(EntityID entityId);

		// Sets the scene to which the entity belongs
		void setScene(std::shared_ptr<EditorScene> scene);

	private: /* functions */

		Pekan::GUI::GUIWindowProperties getProperties() const override;

		// Hides all component widgets
		void hideComponentWidgets();

		void updateWidgetsFromComponentsOfEntity(entt::entity entity);
		void pushWidgetEditsToComponentsOfEntity(entt::entity entity);

		// Updates visibility of component widgets based on which components the given entity has
		void updateWidgetsVisibility(entt::entity entity);

		// Resolves the currently selected EntityID to an entt::entity by looking it up in the
		// scene's registry. Returns entt::null if no entity is selected, or if the selected
		// entity no longer exists (e.g. it was destroyed since selection).
		entt::entity resolveSelectedEntity() const;

	private: /* variables */

		struct Widgets
		{
			Pekan::GUI::TextWidget_Ptr entityInfoTextWidget = std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::CheckboxWidget_Ptr enabledCheckboxWidget = std::make_shared<Pekan::GUI::CheckboxWidget>();
			Pekan::GUI::SeparatorWidget_Ptr firstSeparatorWidget = std::make_shared<Pekan::GUI::SeparatorWidget>();

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

			Pekan::GUI::ContextMenuWidget_Ptr componentsContextMenuWidget = std::make_shared<Pekan::GUI::ContextMenuWidget>();
		} gui;

		// EntityID of the entity whose properties are currently displayed in this GUI window.
		// Stored as a stable EntityID (rather than entt::entity) - the corresponding entt::entity
		// is resolved on demand from current registry.
		EntityID m_selectedEntityId = INVALID_ENTITY_ID;

		// Pointer to the scene to which the entity belongs
		std::shared_ptr<EditorScene> m_scene;
	};

} // namespace Editor
} // namespace Pekan
