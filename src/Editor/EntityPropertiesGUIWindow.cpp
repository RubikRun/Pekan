#include "EntityPropertiesGUIWindow.h"

#include "EditorScene.h"

#include <vector>
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

#include <glm/gtc/constants.hpp>

using namespace Pekan;
using namespace Pekan::GUI;
using namespace Pekan::Renderer2D;

namespace Pekan
{
namespace Editor
{

	static constexpr float PI = 3.14159265358979323846f;

	bool EntityPropertiesGUIWindow::_init()
	{
		if (m_scene == nullptr)
		{
			PK_LOG_ERROR("Failed to initialize EntityPropertiesGUIWindow because there is no scene attached.", "Pekan");
			return false;
		}

		gui.entityInfoTextWidget->create(this, "No entity selected");
		gui.separatorWidget->create(this);
		gui.addComponentComboBoxWidget->create(this, "", 0, EditorScene::getComponentTypesNames());
		gui.addComponentButtonWidget->create(this, "Add Component");
		gui.componentsSeparatorWidget->create(this);

		// Transform2D
		gui.transform2DHeader->create(this, "Transform2D");
		gui.transform2DPosition->create(this, "Position", glm::vec2(0.0f), -1000.0f, 1000.0f, 0.01f, "%.3f");
		gui.transform2DRotation->create(this, "Rotation", 0.0f, -PI, PI, "%.3f");
		gui.transform2DScale->create(this, "Scale", glm::vec2(1.0f), 0.01f, 100.0f, 0.01f, "%.3f");

		// Sprite
		gui.spriteHeader->create(this, "Sprite");
		gui.spriteWidth->create(this, "Width", 1.0f, 0.01f, 100.0f, "%.3f");
		gui.spriteHeight->create(this, "Height", 1.0f, 0.01f, 100.0f, "%.3f");
		gui.spriteTexCoordMin->create(this, "Tex Coord Min", glm::vec2(0.0f), 0.0f, 1.0f, 0.01f, "%.3f");
		gui.spriteTexCoordMax->create(this, "Tex Coord Max", glm::vec2(1.0f), 0.0f, 1.0f, 0.01f, "%.3f");

		// Camera2D
		gui.camera2DHeader->create(this, "Camera2D");
		gui.camera2DSize->create(this, "Size", glm::vec2(16.0f, 9.0f), 0.1f, 1000.0f, 0.1f, "%.2f");
		gui.camera2DPosition->create(this, "Position", glm::vec2(0.0f), -1000.0f, 1000.0f, 0.01f, "%.3f");
		gui.camera2DRotation->create(this, "Rotation", 0.0f, -PI, PI, "%.3f");
		gui.camera2DZoomLevel->create(this, "Zoom", 1.0f, 0.01f, 10.0f, "%.2f");
		gui.camera2DIsPrimary->create(this, "Primary", true);
		gui.camera2DIsControllable->create(this, "Controllable", true);

		// Line
		gui.lineHeader->create(this, "Line");
		gui.linePointA->create(this, "Point A", glm::vec2(0.0f), -1000.0f, 1000.0f, 0.01f, "%.3f");
		gui.linePointB->create(this, "Point B", glm::vec2(1.0f), -1000.0f, 1000.0f, 0.01f, "%.3f");
		gui.lineColor->create(this, "Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		// RectangleGeometry
		gui.rectangleGeometryHeader->create(this, "RectangleGeometry");
		gui.rectangleGeometryWidth->create(this, "Width", 1.0f, 0.01f, 100.0f, "%.3f");
		gui.rectangleGeometryHeight->create(this, "Height", 1.0f, 0.01f, 100.0f, "%.3f");

		// CircleGeometry
		gui.circleGeometryHeader->create(this, "CircleGeometry");
		gui.circleGeometryRadius->create(this, "Radius", 1.0f, 0.01f, 100.0f, "%.3f");
		gui.circleGeometrySegmentsCount->create(this, "Segments", 32, 3, 128);

		// TriangleGeometry
		gui.triangleGeometryHeader->create(this, "TriangleGeometry");
		gui.triangleGeometryPointA->create(this, "Point A", glm::vec2(-1.0f, 0.0f), -1000.0f, 1000.0f, 0.01f, "%.3f");
		gui.triangleGeometryPointB->create(this, "Point B", glm::vec2(1.0f, 0.0f), -1000.0f, 1000.0f, 0.01f, "%.3f");
		gui.triangleGeometryPointC->create(this, "Point C", glm::vec2(0.0f, 1.0f), -1000.0f, 1000.0f, 0.01f, "%.3f");

		// PolygonGeometry
		gui.polygonGeometryHeader->create(this, "PolygonGeometry");
		gui.polygonGeometryVertexCount->create(this, "Vertices: 0");

		// LineGeometry
		gui.lineGeometryHeader->create(this, "LineGeometry");
		gui.lineGeometryPointA->create(this, "Point A", glm::vec2(0.0f), -1000.0f, 1000.0f, 0.01f, "%.3f");
		gui.lineGeometryPointB->create(this, "Point B", glm::vec2(1.0f), -1000.0f, 1000.0f, 0.01f, "%.3f");
		gui.lineGeometryThickness->create(this, "Thickness", 0.02f, 0.001f, 10.0f, "%.3f");

		// SolidColorMaterial
		gui.solidColorMaterialHeader->create(this, "SolidColorMaterial");
		gui.solidColorMaterialColor->create(this, "Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		// All component widgets start hidden
		updateComponentWidgetsVisibility(entt::null);

		return true;
	}

	void EntityPropertiesGUIWindow::update(double deltaTime)
	{
		PK_ASSERT_QUICK(m_scene != nullptr);

		if (gui.addComponentButtonWidget->isClicked() && m_entity != entt::null)
		{
			const int componentTypeIndex = gui.addComponentComboBoxWidget->getIndex();
			m_scene->addComponent(m_entity, componentTypeIndex);
			updateComponentWidgetsVisibility(m_entity);
			syncComponentToWidgets(m_entity);
		}

		if (m_entity != entt::null)
		{
			updateComponentWidgetsVisibility(m_entity);
			syncWidgetsToComponents(m_entity);
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

		updateComponentWidgetsVisibility(entity);
		syncComponentToWidgets(entity);
	}

	void EntityPropertiesGUIWindow::syncComponentToWidgets(entt::entity entity)
	{
		if (entity == entt::null)
			return;

		entt::registry& registry = m_scene->getRegistry();

		if (registry.all_of<TransformComponent2D>(entity))
		{
			const auto& c = registry.get<TransformComponent2D>(entity);
			gui.transform2DPosition->setValue(c.position);
			gui.transform2DRotation->setValue(c.rotation);
			gui.transform2DScale->setValue(c.scaleFactor);
		}

		if (registry.all_of<SpriteComponent>(entity))
		{
			const auto& c = registry.get<SpriteComponent>(entity);
			gui.spriteWidth->setValue(c.width);
			gui.spriteHeight->setValue(c.height);
			gui.spriteTexCoordMin->setValue(c.textureCoordinatesMin);
			gui.spriteTexCoordMax->setValue(c.textureCoordinatesMax);
		}

		if (registry.all_of<CameraComponent2D>(entity))
		{
			const auto& c = registry.get<CameraComponent2D>(entity);
			gui.camera2DSize->setValue(c.size);
			gui.camera2DPosition->setValue(c.position);
			gui.camera2DRotation->setValue(c.rotation);
			gui.camera2DZoomLevel->setValue(c.zoomLevel);
			gui.camera2DIsPrimary->setChecked(c.isPrimary);
			gui.camera2DIsControllable->setChecked(c.isControllable);
		}

		if (registry.all_of<LineComponent>(entity))
		{
			const auto& c = registry.get<LineComponent>(entity);
			gui.linePointA->setValue(c.pointA);
			gui.linePointB->setValue(c.pointB);
			gui.lineColor->setValue(c.color);
		}

		if (registry.all_of<RectangleGeometryComponent>(entity))
		{
			const auto& c = registry.get<RectangleGeometryComponent>(entity);
			gui.rectangleGeometryWidth->setValue(c.width);
			gui.rectangleGeometryHeight->setValue(c.height);
		}

		if (registry.all_of<CircleGeometryComponent>(entity))
		{
			const auto& c = registry.get<CircleGeometryComponent>(entity);
			gui.circleGeometryRadius->setValue(c.radius);
			gui.circleGeometrySegmentsCount->setValue(c.segmentsCount);
		}

		if (registry.all_of<TriangleGeometryComponent>(entity))
		{
			const auto& c = registry.get<TriangleGeometryComponent>(entity);
			gui.triangleGeometryPointA->setValue(c.pointA);
			gui.triangleGeometryPointB->setValue(c.pointB);
			gui.triangleGeometryPointC->setValue(c.pointC);
		}

		if (registry.all_of<PolygonGeometryComponent>(entity))
		{
			const auto& c = registry.get<PolygonGeometryComponent>(entity);
			gui.polygonGeometryVertexCount->setText("Vertices: " + std::to_string(static_cast<int>(c.vertexPositions.size())));
		}

		if (registry.all_of<LineGeometryComponent>(entity))
		{
			const auto& c = registry.get<LineGeometryComponent>(entity);
			gui.lineGeometryPointA->setValue(c.pointA);
			gui.lineGeometryPointB->setValue(c.pointB);
			gui.lineGeometryThickness->setValue(c.thickness);
		}

		if (registry.all_of<SolidColorMaterialComponent>(entity))
		{
			const auto& c = registry.get<SolidColorMaterialComponent>(entity);
			gui.solidColorMaterialColor->setValue(c.color);
		}
	}

	void EntityPropertiesGUIWindow::syncWidgetsToComponents(entt::entity entity)
	{
		entt::registry& registry = m_scene->getRegistry();

		if (registry.all_of<TransformComponent2D>(entity))
		{
			auto& c = registry.get<TransformComponent2D>(entity);
			c.position = gui.transform2DPosition->getValue();
			c.rotation = gui.transform2DRotation->getValue();
			c.scaleFactor = gui.transform2DScale->getValue();
		}

		if (registry.all_of<SpriteComponent>(entity))
		{
			auto& c = registry.get<SpriteComponent>(entity);
			c.width = gui.spriteWidth->getValue();
			c.height = gui.spriteHeight->getValue();
			c.textureCoordinatesMin = gui.spriteTexCoordMin->getValue();
			c.textureCoordinatesMax = gui.spriteTexCoordMax->getValue();
		}

		if (registry.all_of<CameraComponent2D>(entity))
		{
			auto& c = registry.get<CameraComponent2D>(entity);
			c.size = gui.camera2DSize->getValue();
			c.position = gui.camera2DPosition->getValue();
			c.rotation = gui.camera2DRotation->getValue();
			c.zoomLevel = gui.camera2DZoomLevel->getValue();
			c.isPrimary = gui.camera2DIsPrimary->isChecked();
			c.isControllable = gui.camera2DIsControllable->isChecked();
		}

		if (registry.all_of<LineComponent>(entity))
		{
			auto& c = registry.get<LineComponent>(entity);
			c.pointA = gui.linePointA->getValue();
			c.pointB = gui.linePointB->getValue();
			c.color = gui.lineColor->getValue();
		}

		if (registry.all_of<RectangleGeometryComponent>(entity))
		{
			auto& c = registry.get<RectangleGeometryComponent>(entity);
			c.width = gui.rectangleGeometryWidth->getValue();
			c.height = gui.rectangleGeometryHeight->getValue();
		}

		if (registry.all_of<CircleGeometryComponent>(entity))
		{
			auto& c = registry.get<CircleGeometryComponent>(entity);
			c.radius = gui.circleGeometryRadius->getValue();
			c.segmentsCount = gui.circleGeometrySegmentsCount->getValue();
		}

		if (registry.all_of<TriangleGeometryComponent>(entity))
		{
			auto& c = registry.get<TriangleGeometryComponent>(entity);
			c.pointA = gui.triangleGeometryPointA->getValue();
			c.pointB = gui.triangleGeometryPointB->getValue();
			c.pointC = gui.triangleGeometryPointC->getValue();
		}

		if (registry.all_of<LineGeometryComponent>(entity))
		{
			auto& c = registry.get<LineGeometryComponent>(entity);
			c.pointA = gui.lineGeometryPointA->getValue();
			c.pointB = gui.lineGeometryPointB->getValue();
			c.thickness = gui.lineGeometryThickness->getValue();
		}

		if (registry.all_of<SolidColorMaterialComponent>(entity))
		{
			auto& c = registry.get<SolidColorMaterialComponent>(entity);
			c.color = gui.solidColorMaterialColor->getValue();
		}
	}

	void EntityPropertiesGUIWindow::updateComponentWidgetsVisibility(entt::entity entity)
	{
		entt::registry* registry = (m_scene != nullptr) ? &m_scene->getRegistry() : nullptr;
		const bool hasEntity = (registry != nullptr && entity != entt::null);

		auto setVisible = [hasEntity](GUI::Widget* header, std::vector<GUI::Widget*> widgets, bool hasComponent)
		{
			const bool visible = hasEntity && hasComponent;
			visible ? header->unhide() : header->hide();
			for (GUI::Widget* w : widgets)
				visible ? w->unhide() : w->hide();
		};

		setVisible(gui.transform2DHeader.get(), { gui.transform2DPosition.get(), gui.transform2DRotation.get(), gui.transform2DScale.get() },
			hasEntity && registry->all_of<TransformComponent2D>(entity));
		setVisible(gui.spriteHeader.get(), { gui.spriteWidth.get(), gui.spriteHeight.get(), gui.spriteTexCoordMin.get(), gui.spriteTexCoordMax.get() },
			hasEntity && registry->all_of<SpriteComponent>(entity));
		setVisible(gui.camera2DHeader.get(), { gui.camera2DSize.get(), gui.camera2DPosition.get(), gui.camera2DRotation.get(), gui.camera2DZoomLevel.get(), gui.camera2DIsPrimary.get(), gui.camera2DIsControllable.get() },
			hasEntity && registry->all_of<CameraComponent2D>(entity));
		setVisible(gui.lineHeader.get(), { gui.linePointA.get(), gui.linePointB.get(), gui.lineColor.get() },
			hasEntity && registry->all_of<LineComponent>(entity));
		setVisible(gui.rectangleGeometryHeader.get(), { gui.rectangleGeometryWidth.get(), gui.rectangleGeometryHeight.get() },
			hasEntity && registry->all_of<RectangleGeometryComponent>(entity));
		setVisible(gui.circleGeometryHeader.get(), { gui.circleGeometryRadius.get(), gui.circleGeometrySegmentsCount.get() },
			hasEntity && registry->all_of<CircleGeometryComponent>(entity));
		setVisible(gui.triangleGeometryHeader.get(), { gui.triangleGeometryPointA.get(), gui.triangleGeometryPointB.get(), gui.triangleGeometryPointC.get() },
			hasEntity && registry->all_of<TriangleGeometryComponent>(entity));
		setVisible(gui.polygonGeometryHeader.get(), { gui.polygonGeometryVertexCount.get() },
			hasEntity && registry->all_of<PolygonGeometryComponent>(entity));
		setVisible(gui.lineGeometryHeader.get(), { gui.lineGeometryPointA.get(), gui.lineGeometryPointB.get(), gui.lineGeometryThickness.get() },
			hasEntity && registry->all_of<LineGeometryComponent>(entity));
		setVisible(gui.solidColorMaterialHeader.get(), { gui.solidColorMaterialColor.get() },
			hasEntity && registry->all_of<SolidColorMaterialComponent>(entity));
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
