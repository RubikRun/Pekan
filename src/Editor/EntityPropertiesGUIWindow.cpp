#include "EntityPropertiesGUIWindow.h"

#include "EditorScene.h"

#include "PekanLogger.h"

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
				// TO-DO 0109: Show a message to user in GUI instead of loggin a warning
				PK_LOG_WARNING("Cannot add a component because no entity is selected.", "Pekan");
			}
		}
		else
		{
			if (m_entity != entt::null)
			{
				updateComponentsOfEntityFromWidgets(m_entity);
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

	void EntityPropertiesGUIWindow::updateComponentsOfEntityFromWidgets(entt::entity entity)
	{
		PK_ASSERT_QUICK(m_scene != nullptr);
		PK_ASSERT_QUICK(entity != entt::null);

		entt::registry& registry = m_scene->getRegistry();

		if (registry.all_of<TransformComponent2D>(entity))
		{
			auto& c = registry.get<TransformComponent2D>(entity);
			c.position = gui.transform2DWidgets.position->getValue();
			c.rotation = gui.transform2DWidgets.rotation->getValue();
			c.scaleFactor = gui.transform2DWidgets.scale->getValue();
		}

		if (registry.all_of<SpriteComponent>(entity))
		{
			auto& c = registry.get<SpriteComponent>(entity);
			c.width = gui.spriteWidgets.width->getValue();
			c.height = gui.spriteWidgets.height->getValue();
			c.textureCoordinatesMin = gui.spriteWidgets.texCoordMin->getValue();
			c.textureCoordinatesMax = gui.spriteWidgets.texCoordMax->getValue();
		}

		if (registry.all_of<RectangleGeometryComponent>(entity))
		{
			auto& c = registry.get<RectangleGeometryComponent>(entity);
			c.width = gui.rectangleGeometryWidgets.width->getValue();
			c.height = gui.rectangleGeometryWidgets.height->getValue();
		}

		if (registry.all_of<CircleGeometryComponent>(entity))
		{
			auto& c = registry.get<CircleGeometryComponent>(entity);
			c.radius = gui.circleGeometryWidgets.radius->getValue();
			c.segmentsCount = gui.circleGeometryWidgets.segmentsCount->getValue();
		}

		if (registry.all_of<TriangleGeometryComponent>(entity))
		{
			auto& c = registry.get<TriangleGeometryComponent>(entity);
			c.pointA = gui.triangleGeometryWidgets.pointA->getValue();
			c.pointB = gui.triangleGeometryWidgets.pointB->getValue();
			c.pointC = gui.triangleGeometryWidgets.pointC->getValue();
		}

		// Intentionally skipping PolygonGeometry here,
		// because for now it only has a widget for showing vertex count and it's read-only

		if (registry.all_of<LineGeometryComponent>(entity))
		{
			auto& c = registry.get<LineGeometryComponent>(entity);
			c.pointA = gui.lineGeometryWidgets.pointA->getValue();
			c.pointB = gui.lineGeometryWidgets.pointB->getValue();
			c.thickness = gui.lineGeometryWidgets.thickness->getValue();
		}

		if (registry.all_of<SolidColorMaterialComponent>(entity))
		{
			auto& c = registry.get<SolidColorMaterialComponent>(entity);
			c.color = gui.solidColorMaterialWidgets.color->getValue();
		}

		if (registry.all_of<LineComponent>(entity))
		{
			auto& c = registry.get<LineComponent>(entity);
			c.pointA = gui.lineWidgets.pointA->getValue();
			c.pointB = gui.lineWidgets.pointB->getValue();
			c.color = gui.lineWidgets.color->getValue();
		}

		if (registry.all_of<CameraComponent2D>(entity))
		{
			auto& c = registry.get<CameraComponent2D>(entity);
			c.size = gui.camera2DWidgets.size->getValue();
			c.position = gui.camera2DWidgets.position->getValue();
			c.rotation = gui.camera2DWidgets.rotation->getValue();
			c.zoomLevel = gui.camera2DWidgets.zoomLevel->getValue();
			c.isPrimary = gui.camera2DWidgets.isPrimary->isChecked();
			c.isControllable = gui.camera2DWidgets.isControllable->isChecked();
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
