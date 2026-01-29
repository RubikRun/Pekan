#include "Demo07_Scene.h"

#include "Demo07_GUIWindow.h"
#include "PekanLogger.h"
#include "RenderState.h"

#include "CameraComponent2D.h"
#include "TransformComponent2D.h"
#include "LineComponent.h"
#include "LineGeometryComponent.h"
#include "RectangleGeometryComponent.h"
#include "CircleGeometryComponent.h"
#include "TriangleGeometryComponent.h"
#include "PolygonGeometryComponent.h"
#include "SolidColorMaterialComponent.h"

using namespace Pekan;
using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;

constexpr float CAMERA_SCALE = 10.0f;
constexpr float DIVS_LENGTH = 0.1f;
constexpr glm::vec4 COORD_SYS_COLOR = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);

namespace Demo
{

    bool Demo07_Scene::_init()
	{
		RenderState::enableMultisampleAntiAliasing();

		createCamera();
		createCoordSys();
		createShapes();

		return true;
	}

	void Demo07_Scene::_exit()
	{
		destroyEntity(m_line);
		destroyEntity(m_polygon);
		destroyEntity(m_triangle);
		destroyEntity(m_circle);
		destroyEntity(m_rectangle);

		for (entt::entity coordSysLine : m_coordSys)
		{
			destroyEntity(coordSysLine);
		}

		destroyEntity(m_camera);
	}

	void Demo07_Scene::update(double dt)
	{
		TransformComponent2D& rectangleTransform = m_registry.get<TransformComponent2D>(m_rectangle);
		TransformComponent2D& circleTransform = m_registry.get<TransformComponent2D>(m_circle);
		TransformComponent2D& triangleTransform = m_registry.get<TransformComponent2D>(m_triangle);
		TransformComponent2D& polygonTransform = m_registry.get<TransformComponent2D>(m_polygon);
		TransformComponent2D& lineTransform = m_registry.get<TransformComponent2D>(m_line);

		rectangleTransform.position = m_guiWindow->getRectanglePosition();
		rectangleTransform.rotation = m_guiWindow->getRectangleRotation();
		rectangleTransform.scaleFactor = m_guiWindow->getRectangleScale();

		circleTransform.position = m_guiWindow->getCirclePosition();
		circleTransform.rotation = m_guiWindow->getCircleRotation();
		circleTransform.scaleFactor = m_guiWindow->getCircleScale();

		triangleTransform.position = m_guiWindow->getTrianglePosition();
		triangleTransform.rotation = m_guiWindow->getTriangleRotation();
		triangleTransform.scaleFactor = m_guiWindow->getTriangleScale();

		polygonTransform.position = m_guiWindow->getPolygonPosition();
		polygonTransform.rotation = m_guiWindow->getPolygonRotation();
		polygonTransform.scaleFactor = m_guiWindow->getPolygonScale();

		lineTransform.position = m_guiWindow->getLinePosition();
		lineTransform.rotation = m_guiWindow->getLineRotation();
		lineTransform.scaleFactor = m_guiWindow->getLineScale();
	}

	void Demo07_Scene::createCamera()
	{
		m_camera = createEntity();
		// Add camera component to camera entity
		CameraComponent2D cameraComponent;
		cameraComponent.setHeight(CAMERA_SCALE, true);
		m_registry.emplace<CameraComponent2D>(m_camera, cameraComponent);
	}

	void Demo07_Scene::createCoordSys()
	{
		// Number of divisions rendered on the coordinate system
		// in each of the 4 directions, starting from the center
		static constexpr int divsCount = int(CAMERA_SCALE) * 10;

		m_coordSys.resize(divsCount * 4 + 2);
		// Create horizontal and vertical line
		m_coordSys[0] = createEntity();
		m_registry.emplace<LineComponent>(m_coordSys[0], glm::vec2{ -9999.0f, 0.0f }, glm::vec2{ 9999.0f, 0.0f }, COORD_SYS_COLOR);
		m_coordSys[1] = createEntity();
		m_registry.emplace<LineComponent>(m_coordSys[1], glm::vec2{ 0.0f, -9999.0f }, glm::vec2{ 0.0f, 9999.0f }, COORD_SYS_COLOR);
		// Create division lines in 4 directions
		for (int i = 0; i < divsCount; i++)
		{
			m_coordSys[2 + i] = createEntity();
			m_registry.emplace<LineComponent>(m_coordSys[2 + i], glm::vec2{ float(i + 1), -DIVS_LENGTH }, glm::vec2{ float(i + 1), DIVS_LENGTH }, COORD_SYS_COLOR);
		}
		for (int i = 0; i < divsCount; i++)
		{
			m_coordSys[2 + divsCount + i] = createEntity();
			m_registry.emplace<LineComponent>(m_coordSys[2 + divsCount + i], glm::vec2{ float(-1 - i), -DIVS_LENGTH }, glm::vec2{ float(-1 - i), DIVS_LENGTH }, COORD_SYS_COLOR);
		}
		for (int i = 0; i < divsCount; i++)
		{
			m_coordSys[2 + divsCount * 2 + i] = createEntity();
			m_registry.emplace<LineComponent>(m_coordSys[2 + divsCount * 2 + i], glm::vec2{ -DIVS_LENGTH, float(i + 1) }, glm::vec2{ DIVS_LENGTH, float(i + 1) }, COORD_SYS_COLOR);
		}
		for (int i = 0; i < divsCount; i++)
		{
			m_coordSys[2 + divsCount * 3 + i] = createEntity();
			m_registry.emplace<LineComponent>(m_coordSys[2 + divsCount * 3 + i], glm::vec2{ -DIVS_LENGTH, float(-1 - i) }, glm::vec2{ DIVS_LENGTH, float(-1 - i) }, COORD_SYS_COLOR);
		}
	}

	void Demo07_Scene::createShapes()
	{
		m_rectangle = createEntity();
		m_registry.emplace<TransformComponent2D>(m_rectangle, glm::vec2{ 1.5f, 1.0f });
		m_registry.emplace<RectangleGeometryComponent>(m_rectangle, 1.0f, 1.5f);
		m_registry.emplace<SolidColorMaterialComponent>(m_rectangle, glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		m_circle = createEntity();
		m_registry.emplace<TransformComponent2D>(m_circle, glm::vec2{ -1.5f, 1.0f });
		m_registry.emplace<CircleGeometryComponent>(m_circle, 0.6f);
		m_registry.emplace<SolidColorMaterialComponent>(m_circle, glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

		m_triangle = createEntity();
		m_registry.emplace<TransformComponent2D>(m_triangle, glm::vec2{ 1.5f, -1.0f });
		m_registry.emplace<TriangleGeometryComponent>(m_triangle, glm::vec2(-0.5, -0.5f), glm::vec2(0.5f, -0.5f), glm::vec2(0.0f, 0.5f));
		m_registry.emplace<SolidColorMaterialComponent>(m_triangle, glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });

		m_polygon = createEntity();
		m_registry.emplace<TransformComponent2D>(m_polygon, glm::vec2{ -1.5f, -2.0f });
		m_registry.emplace<PolygonGeometryComponent>(m_polygon, std::vector<glm::vec2>{
			{  0.3f,  0.6f },
			{  0.1f,  0.9f },
			{ -0.3f,  0.8f },
			{ -0.5f,  0.3f },
			{ -0.1f, -0.2f },
			{  0.5f,  0.0f }
		});
		m_registry.emplace<SolidColorMaterialComponent>(m_polygon, glm::vec4{ 1.0f, 1.0f, 0.0f, 1.0f });

		m_line = createEntity();
		m_registry.emplace<TransformComponent2D>(m_line, glm::vec2{ 0.5f, 0.5f });
		m_registry.emplace<LineGeometryComponent>(m_line, glm::vec2{ -2.0f, -1.2f }, glm::vec2{ 1.2f, 2.0f }, 0.02f);
		m_registry.emplace<SolidColorMaterialComponent>(m_line, glm::vec4{ 0.0f, 1.0f, 1.0f, 1.0f });
	}

} // namespace Demo