#include "Demo10_Scene.h"

#include "Demo10_GUIWindow.h"
#include "CameraComponent2D.h"
#include "CameraSystem2D.h"
#include "RenderState.h"
#include "PekanLogger.h"
#include "Events/MouseEvents.h"

using namespace Pekan::Renderer2D;
using namespace Pekan::Graphics;
using namespace Pekan;

namespace Demo
{

	constexpr float CAMERA_SCALE = 10.0f;

	bool Demo10_Scene::_init()
	{
		if (m_guiWindow == nullptr)
		{
			PK_LOG_ERROR("Cannot initialize Demo10_Scene because there is no GUI window attached.", "Demo10");
			return false;
		}

		RenderState::enableMultisampleAntiAliasing();
		RenderState::enableBlending();
		RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

		m_player.create(this);

		createCamera();

		return true;
	}

	void Demo10_Scene::_exit()
	{
		destroyEntity(m_camera);
		m_player.destroy();
	}

	void Demo10_Scene::update(double deltaTime)
	{
		PK_ASSERT_QUICK(m_guiWindow != nullptr);
		m_player.update(m_guiWindow.get());
	}

	void Demo10_Scene::createCamera()
	{
		m_camera = createEntity();
		// Add camera component to camera entity
		CameraComponent2D cameraComponent;
		cameraComponent.setHeight(CAMERA_SCALE);
		cameraComponent.position = glm::vec2{ 0.0f, -0.5f };
		cameraComponent.zoomLevel = 0.9f;
		getRegistry().emplace<CameraComponent2D>(m_camera, cameraComponent);
	}

	bool Demo10_Scene::onMouseButtonPressed(const MouseButtonPressedEvent& event)
	{
		if (event.getMouseButton() == Pekan::MouseButton::Left)
		{
			CameraComponent2D& cameraComponent = getRegistry().get<CameraComponent2D>(m_camera);
			const glm::vec2 mousePos = PekanEngine::getMousePosition();
			const glm::vec2 mousePosInWorld = cameraComponent.windowToWorldPosition(mousePos);
			PK_LOG_INFO("position: " << mousePosInWorld.x << ", " << mousePosInWorld.y, "Demo10");
			return true;
		}
		return false;
	}

} // namespace Demo
