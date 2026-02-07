#include "Demo05_Scene.h"

#include "Demo05_GUIWindow.h"
#include "PekanLogger.h"
#include "Utils/RandomizationUtils.h"
#include "RenderState.h"
#include "Renderer2DSubsystem.h"

#include "CameraComponent2D.h"
#include "LineComponent.h"
#include "LineGeometryComponent.h"
#include "SolidColorMaterialComponent.h"

#include <glm/gtc/constants.hpp>
constexpr float PI = glm::pi<float>();

// Parameters for Mandelbrot Times Table
constexpr glm::vec2 MTT0_CENTER = glm::vec2(-0.5f, 0.0f);
constexpr glm::vec2 MTT1_CENTER = glm::vec2(0.5f, 0.0f);
constexpr float MTT0_RADIUS = 0.45f;
constexpr float MTT1_RADIUS = 0.45f;
constexpr float MTT0_SPEED = 0.01f;
constexpr float MTT1_SPEED = 0.01f;

constexpr float CAMERA_SCALE = MTT0_RADIUS * 5.0f;

using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;

namespace Demo
{

	bool Demo05_Scene::_init()
	{
		RenderState::enableMultisampleAntiAliasing();
		// Enable and configure blending
		RenderState::enableBlending();
		RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

		// Create lines for MTT0 (entities with LineComponent)
		for (int i = 0; i < MTT0_SUBDIVS; i++)
		{
			m_lines[i] = createEntity();
			m_registry.emplace<LineComponent>(m_lines[i], glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 0.0f, 0.0f }, glm::vec4{ 0.9f, 0.9f, 0.9f, 1.0f });
		}
		// Create lines for MTT1 (entities with LineGeometryComponent and SolidColorMaterialComponent)
		for (int i = 0; i < MTT1_SUBDIVS; i++)
		{
			m_lineShapes[i] = createEntity();
			m_registry.emplace<LineGeometryComponent>(m_lineShapes[i], glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 0.0f, 0.0f }, 0.001f);
			m_registry.emplace<SolidColorMaterialComponent>(m_lineShapes[i], glm::vec4{ 0.9f, 0.9f, 0.9f, 1.0f });
		}

		// Create camera
		m_camera = createEntity();
		CameraComponent2D cameraComponent;
		cameraComponent.setHeight(CAMERA_SCALE, true);
		m_registry.emplace<CameraComponent2D>(m_camera, cameraComponent);

		t = 0.0f;

		return true;
	}

	void Demo05_Scene::_exit()
	{
		for (int i = 0; i < MTT0_SUBDIVS; i++)
		{
			destroyEntity(m_lines[i]);
		}
		for (int i = 0; i < MTT1_SUBDIVS; i++)
		{
			destroyEntity(m_lineShapes[i]);
		}
	}

	void Demo05_Scene::update(double dt)
	{
		PK_ASSERT(m_guiWindow != nullptr, "Cannot update Demo05_Scene because there is no Demo05_GUIWindow attached.", "Demo05");
		// Set background color from GUI window
		const glm::vec3 backgroundColor = m_guiWindow->getBackgroundColor();
		RenderState::setBackgroundColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);

		updateMtt();

		t += float(dt);
		m_mtt0factor += float(dt) * MTT0_SPEED;
		m_mtt1factor += float(dt) * MTT1_SPEED;
	}

	static float osc(float x)
	{
		return cos(x) * 0.5f + 0.5f;
	}
	static float osc(float x, float a, float b)
	{
		return a + (b - a) * osc(x);
	}

	static glm::vec2 getMtt0Point(float ang)
	{
		return glm::vec2
		(
			cos(ang) * MTT0_RADIUS + MTT0_CENTER.x,
			sin(ang) * MTT0_RADIUS + MTT0_CENTER.y
		);
	}

	static glm::vec2 getMtt1Point(float ang)
	{
		return glm::vec2
		(
			cos(ang) * MTT1_RADIUS + MTT1_CENTER.x,
			sin(ang) * MTT1_RADIUS + MTT1_CENTER.y
		);
	}

	void Demo05_Scene::updateMtt()
	{
		static constexpr float arc0 = 2.0f * PI / float(MTT0_SUBDIVS);
		for (int i = 0; i < MTT0_SUBDIVS; i++)
		{
			LineComponent& lineComponent = m_registry.get<LineComponent>(m_lines[i]);

			const float angA = float(i) * arc0;
			const float angB = float(i) * m_mtt0factor * arc0;
			lineComponent.pointA = getMtt0Point(angA);
			lineComponent.pointB = getMtt0Point(angB);
		}

		static constexpr float arc1 = 2.0f * PI / float(MTT1_SUBDIVS);
		for (int i = 0; i < MTT1_SUBDIVS; i++)
		{
			LineGeometryComponent& lineGeometryComponent = m_registry.get<LineGeometryComponent>(m_lineShapes[i]);

			const float angA = float(i) * arc1;
			const float angB = float(i) * m_mtt1factor * arc1;
			lineGeometryComponent.pointA = getMtt1Point(angA);
			lineGeometryComponent.pointB = getMtt1Point(angB);

			if (m_guiWindow != nullptr)
			{
				lineGeometryComponent.thickness = m_guiWindow->getLineThickness();
			}
		}
	}

} // namespace Demo