#include "EntityPropertiesGUIWindow.h"

#include "EditorScene.h"

#include "PekanLogger.h"
#include "PekanUserMessageBox.h"

#include "TransformComponent2D.h"
#include "SpriteComponent.h"
#include "CameraComponent2D.h"
#include "LineComponent.h"
#include "RectangleGeometryComponent.h"
#include "CircleGeometryComponent.h"
#include "TriangleGeometryComponent.h"
#include "PolygonGeometryComponent.h"
#include "LineGeometryComponent.h"
#include "SolidColorMaterialComponent.h"

using namespace Pekan;
using namespace Pekan::GUI;
using namespace Pekan::Renderer2D;

namespace Pekan
{
namespace Editor
{

	bool EntityPropertiesGUIWindow::_init()
	{
		if (m_scene == nullptr)
		{
			PK_LOG_ERROR("Failed to initialize EntityPropertiesGUIWindow because there is no scene attached.", "Pekan");
			return false;
		}

		gui.entityInfoTextWidget->create(this, "No entity selected");
		gui.firstSeparatorWidget->create(this);
		gui.addComponentComboBoxWidget->create(this, "", 0, EditorScene::getComponentTypesNames());
		gui.addComponentButtonWidget->create(this, "Add Component");
		gui.secondSeparatorWidget->create(this);

		gui.transform2DWidgets.create(this);
		gui.spriteWidgets.create(this);
		gui.rectangleGeometryWidgets.create(this);
		gui.circleGeometryWidgets.create(this);
		gui.triangleGeometryWidgets.create(this);
		gui.polygonGeometryWidgets.create(this);
		gui.lineGeometryWidgets.create(this);
		gui.solidColorMaterialWidgets.create(this);
		gui.lineWidgets.create(this);
		gui.camera2DWidgets.create(this);

		// Hide all component widgets because initially no entity is selected
		hideComponentWidgets();

		return true;
	}

	void EntityPropertiesGUIWindow::update(double deltaTime)
	{
		PK_ASSERT_QUICK(m_scene != nullptr);

		if (gui.addComponentButtonWidget->isClicked())
		{
			if (m_entity != entt::null)
			{
				const int componentTypeIndex = gui.addComponentComboBoxWidget->getIndex();
				m_scene->addComponent(m_entity, componentTypeIndex);
				updateWidgetsVisibility(m_entity);
				updateWidgetsFromComponentsOfEntity(m_entity);
			}
			else
			{
				PK_SHOW_WARNING("Cannot add a component because no entity is selected.");
			}
		}
		else
		{
			if (m_entity != entt::null)
			{
				pushWidgetEditsToComponentsOfEntity(m_entity);
				updateWidgetsFromComponentsOfEntity(m_entity);
			}
		}
	}

	void EntityPropertiesGUIWindow::setEntity(entt::entity entity)
	{
		m_entity = entity;

		std::string entityInfoText;
		if (entity == entt::null)
		{
			entityInfoText = "No entity selected";
		}
		else
		{
			entityInfoText = "Selected Entity: " + std::to_string(uint32_t(entity));
		}
		gui.entityInfoTextWidget->setText(entityInfoText);

		updateWidgetsVisibility(entity);
		updateWidgetsFromComponentsOfEntity(entity);
	}

	void EntityPropertiesGUIWindow::setScene(std::shared_ptr<EditorScene> scene)
	{
		if (scene == nullptr)
		{
			PK_LOG_ERROR("Cannot set a null scene for EntityPropertiesGUIWindow.", "Pekan");
			return;
		}
		m_scene = scene;
	}

	void EntityPropertiesGUIWindow::hideComponentWidgets()
	{
		gui.transform2DWidgets.hide();
		gui.spriteWidgets.hide();
		gui.rectangleGeometryWidgets.hide();
		gui.circleGeometryWidgets.hide();
		gui.triangleGeometryWidgets.hide();
		gui.polygonGeometryWidgets.hide();
		gui.lineGeometryWidgets.hide();
		gui.solidColorMaterialWidgets.hide();
		gui.lineWidgets.hide();
		gui.camera2DWidgets.hide();
	}

	void EntityPropertiesGUIWindow::updateWidgetsFromComponentsOfEntity(entt::entity entity)
	{
		PK_ASSERT_QUICK(m_scene != nullptr);
		PK_ASSERT_QUICK(entity != entt::null);

		const entt::registry& registry = m_scene->getRegistry();

		if (registry.all_of<TransformComponent2D>(entity))
		{
			const auto& c = registry.get<TransformComponent2D>(entity);
			gui.transform2DWidgets.position->setValue(c.position);
			gui.transform2DWidgets.rotation->setValue(c.rotation);
			gui.transform2DWidgets.scale->setValue(c.scaleFactor);
		}

		if (registry.all_of<SpriteComponent>(entity))
		{
			const auto& c = registry.get<SpriteComponent>(entity);
			gui.spriteWidgets.width->setValue(c.width);
			gui.spriteWidgets.height->setValue(c.height);
			gui.spriteWidgets.texCoordMin->setValue(c.textureCoordinatesMin);
			gui.spriteWidgets.texCoordMax->setValue(c.textureCoordinatesMax);
		}

		if (registry.all_of<RectangleGeometryComponent>(entity))
		{
			const auto& c = registry.get<RectangleGeometryComponent>(entity);
			gui.rectangleGeometryWidgets.width->setValue(c.width);
			gui.rectangleGeometryWidgets.height->setValue(c.height);
		}

		if (registry.all_of<CircleGeometryComponent>(entity))
		{
			const auto& c = registry.get<CircleGeometryComponent>(entity);
			gui.circleGeometryWidgets.radius->setValue(c.radius);
			gui.circleGeometryWidgets.segmentsCount->setValue(c.segmentsCount);
		}

		if (registry.all_of<TriangleGeometryComponent>(entity))
		{
			const auto& c = registry.get<TriangleGeometryComponent>(entity);
			gui.triangleGeometryWidgets.pointA->setValue(c.pointA);
			gui.triangleGeometryWidgets.pointB->setValue(c.pointB);
			gui.triangleGeometryWidgets.pointC->setValue(c.pointC);
		}

		if (registry.all_of<PolygonGeometryComponent>(entity))
		{
			const auto& c = registry.get<PolygonGeometryComponent>(entity);
			gui.polygonGeometryWidgets.vertexCount->setText("Vertices: " + std::to_string(static_cast<int>(c.vertexPositions.size())));
		}

		if (registry.all_of<LineGeometryComponent>(entity))
		{
			const auto& c = registry.get<LineGeometryComponent>(entity);
			gui.lineGeometryWidgets.pointA->setValue(c.pointA);
			gui.lineGeometryWidgets.pointB->setValue(c.pointB);
			gui.lineGeometryWidgets.thickness->setValue(c.thickness);
		}

		if (registry.all_of<SolidColorMaterialComponent>(entity))
		{
			const auto& c = registry.get<SolidColorMaterialComponent>(entity);
			gui.solidColorMaterialWidgets.color->setValue(c.color);
		}

		if (registry.all_of<LineComponent>(entity))
		{
			const auto& c = registry.get<LineComponent>(entity);
			gui.lineWidgets.pointA->setValue(c.pointA);
			gui.lineWidgets.pointB->setValue(c.pointB);
			gui.lineWidgets.color->setValue(c.color);
		}

		if (registry.all_of<CameraComponent2D>(entity))
		{
			const auto& c = registry.get<CameraComponent2D>(entity);
			gui.camera2DWidgets.size->setValue(c.size);
			gui.camera2DWidgets.position->setValue(c.position);
			gui.camera2DWidgets.rotation->setValue(c.rotation);
			gui.camera2DWidgets.zoomLevel->setValue(c.zoomLevel);
			gui.camera2DWidgets.isPrimary->setChecked(c.isPrimary);
			gui.camera2DWidgets.isControllable->setChecked(c.isControllable);
		}
	}

	void EntityPropertiesGUIWindow::pushWidgetEditsToComponentsOfEntity(entt::entity entity)
	{
		PK_ASSERT_QUICK(m_scene != nullptr);
		PK_ASSERT_QUICK(entity != entt::null);

		entt::registry& registry = m_scene->getRegistry();

		if (registry.all_of<TransformComponent2D>(entity))
		{
			Transform2DWidgets& widgets = gui.transform2DWidgets;
			if (widgets.position->wasChangedByUserSinceLastAccess() || widgets.rotation->wasChangedByUserSinceLastAccess() || widgets.scale->wasChangedByUserSinceLastAccess())
			{
				auto& c = registry.get<TransformComponent2D>(entity);
				c.position = widgets.position->getValue();
				c.rotation = widgets.rotation->getValue();
				c.scaleFactor = widgets.scale->getValue();
			}
		}

		if (registry.all_of<SpriteComponent>(entity))
		{
			SpriteWidgets& widgets = gui.spriteWidgets;
			if (widgets.width->wasChangedByUserSinceLastAccess() || widgets.height->wasChangedByUserSinceLastAccess() || widgets.texCoordMin->wasChangedByUserSinceLastAccess() || widgets.texCoordMax->wasChangedByUserSinceLastAccess())
			{
				auto& c = registry.get<SpriteComponent>(entity);
				c.width = widgets.width->getValue();
				c.height = widgets.height->getValue();
				c.textureCoordinatesMin = widgets.texCoordMin->getValue();
				c.textureCoordinatesMax = widgets.texCoordMax->getValue();
			}
		}

		if (registry.all_of<RectangleGeometryComponent>(entity))
		{
			RectangleGeometryWidgets& widgets = gui.rectangleGeometryWidgets;
			if (widgets.width->wasChangedByUserSinceLastAccess() || widgets.height->wasChangedByUserSinceLastAccess())
			{
				auto& c = registry.get<RectangleGeometryComponent>(entity);
				c.width = widgets.width->getValue();
				c.height = widgets.height->getValue();
			}
		}

		if (registry.all_of<CircleGeometryComponent>(entity))
		{
			CircleGeometryWidgets& widgets = gui.circleGeometryWidgets;
			if (widgets.radius->wasChangedByUserSinceLastAccess() || widgets.segmentsCount->wasChangedByUserSinceLastAccess())
			{
				auto& c = registry.get<CircleGeometryComponent>(entity);
				c.radius = widgets.radius->getValue();
				c.segmentsCount = widgets.segmentsCount->getValue();
			}
		}

		if (registry.all_of<TriangleGeometryComponent>(entity))
		{
			TriangleGeometryWidgets& widgets = gui.triangleGeometryWidgets;
			if (widgets.pointA->wasChangedByUserSinceLastAccess() || widgets.pointB->wasChangedByUserSinceLastAccess() || widgets.pointC->wasChangedByUserSinceLastAccess())
			{
				auto& c = registry.get<TriangleGeometryComponent>(entity);
				c.pointA = widgets.pointA->getValue();
				c.pointB = widgets.pointB->getValue();
				c.pointC = widgets.pointC->getValue();
			}
		}

		// Intentionally skipping PolygonGeometry here,
		// because for now it only has a widget for showing vertex count and it's read-only

		if (registry.all_of<LineGeometryComponent>(entity))
		{
			LineGeometryWidgets& widgets = gui.lineGeometryWidgets;
			if (widgets.pointA->wasChangedByUserSinceLastAccess() || widgets.pointB->wasChangedByUserSinceLastAccess() || widgets.thickness->wasChangedByUserSinceLastAccess())
			{
				auto& c = registry.get<LineGeometryComponent>(entity);
				c.pointA = widgets.pointA->getValue();
				c.pointB = widgets.pointB->getValue();
				c.thickness = widgets.thickness->getValue();
			}
		}

		if (registry.all_of<SolidColorMaterialComponent>(entity))
		{
			SolidColorMaterialWidgets& widgets = gui.solidColorMaterialWidgets;
			if (widgets.color->wasChangedByUserSinceLastAccess())
			{
				auto& c = registry.get<SolidColorMaterialComponent>(entity);
				c.color = widgets.color->getValue();
			}
		}

		if (registry.all_of<LineComponent>(entity))
		{
			LineWidgets& widgets = gui.lineWidgets;
			if (widgets.pointA->wasChangedByUserSinceLastAccess() || widgets.pointB->wasChangedByUserSinceLastAccess() || widgets.color->wasChangedByUserSinceLastAccess())
			{
				auto& c = registry.get<LineComponent>(entity);
				c.pointA = widgets.pointA->getValue();
				c.pointB = widgets.pointB->getValue();
				c.color = widgets.color->getValue();
			}
		}

		if (registry.all_of<CameraComponent2D>(entity))
		{
			Camera2DWidgets& widgets = gui.camera2DWidgets;
			if (widgets.size->wasChangedByUserSinceLastAccess() || widgets.position->wasChangedByUserSinceLastAccess() || widgets.rotation->wasChangedByUserSinceLastAccess() || widgets.zoomLevel->wasChangedByUserSinceLastAccess() || widgets.isPrimary->wasChangedByUserSinceLastAccess() || widgets.isControllable->wasChangedByUserSinceLastAccess())
			{
				auto& c = registry.get<CameraComponent2D>(entity);
				c.size = widgets.size->getValue();
				c.position = widgets.position->getValue();
				c.rotation = widgets.rotation->getValue();
				c.zoomLevel = widgets.zoomLevel->getValue();
				c.isPrimary = widgets.isPrimary->isChecked();
				c.isControllable = widgets.isControllable->isChecked();
			}
		}
	}

	void EntityPropertiesGUIWindow::updateWidgetsVisibility(entt::entity entity)
	{
		PK_ASSERT_QUICK(m_scene != nullptr);

		const entt::registry& registry = m_scene->getRegistry();

		if (entity == entt::null)
		{
			hideComponentWidgets();
			return;
		}

		if (registry.all_of<TransformComponent2D>(entity))
		{
			gui.transform2DWidgets.unhide();
		}
		else
		{
			gui.transform2DWidgets.hide();
		}

		if (registry.all_of<SpriteComponent>(entity))
		{
			gui.spriteWidgets.unhide();
		}
		else
		{
			gui.spriteWidgets.hide();
		}

		if (registry.all_of<RectangleGeometryComponent>(entity))
		{
			gui.rectangleGeometryWidgets.unhide();
		}
		else
		{
			gui.rectangleGeometryWidgets.hide();
		}

		if (registry.all_of<CircleGeometryComponent>(entity))
		{
			gui.circleGeometryWidgets.unhide();
		}
		else
		{
			gui.circleGeometryWidgets.hide();
		}

		if (registry.all_of<TriangleGeometryComponent>(entity))
		{
			gui.triangleGeometryWidgets.unhide();
		}
		else
		{
			gui.triangleGeometryWidgets.hide();
		}

		if (registry.all_of<PolygonGeometryComponent>(entity))
		{
			gui.polygonGeometryWidgets.unhide();
		}
		else
		{
			gui.polygonGeometryWidgets.hide();
		}

		if (registry.all_of<LineGeometryComponent>(entity))
		{
			gui.lineGeometryWidgets.unhide();
		}
		else
		{
			gui.lineGeometryWidgets.hide();
		}

		if (registry.all_of<SolidColorMaterialComponent>(entity))
		{
			gui.solidColorMaterialWidgets.unhide();
		}
		else
		{
			gui.solidColorMaterialWidgets.hide();
		}

		if (registry.all_of<LineComponent>(entity))
		{
			gui.lineWidgets.unhide();
		}
		else
		{
			gui.lineWidgets.hide();
		}

		if (registry.all_of<CameraComponent2D>(entity))
		{
			gui.camera2DWidgets.unhide();
		}
		else
		{
			gui.camera2DWidgets.hide();
		}
	}

	GUIWindowProperties EntityPropertiesGUIWindow::getProperties() const
	{
		GUIWindowProperties props;
		props.size = { 400, 700 };
		props.name = "Entity Properties";
		return props;
	}

} // namespace Editor
} // namespace Pekan
