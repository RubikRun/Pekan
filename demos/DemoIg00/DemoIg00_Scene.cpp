#include "DemoIg00_Scene.h"

#include "PekanApplication.h"
#include "Events/KeyEvents.h"
#include "RenderCommands.h"
#include "RenderState.h"
#include "Renderer2DSystem.h"
#include "SwimmingDotsRenderer.h"
#include "FunctionGraphsRenderer.h"

using Pekan::KeyPressedEvent;
using Pekan::KeyCode;

using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;

namespace Demo
{

	// Portrait phone framing: taller than wide. Player ~25-30% of view height.
	static constexpr float CAMERA_WORLD_HEIGHT = 7.0f;
	static constexpr float CAMERA_WORLD_WIDTH = CAMERA_WORLD_HEIGHT * (9.0f / 16.0f);
	static constexpr float GROUND_HEIGHT = 1.0f;
	static constexpr float GROUND_CENTER_Y = -2.5f;
	static constexpr float PLAYER_HEIGHT = 2.0f;
	// Ninja sprites are ~232x439
	static constexpr float PLAYER_WIDTH = PLAYER_HEIGHT * (232.0f / 439.0f);

	static constexpr float PORTAL_WIDTH = 0.55f;
	static constexpr float PORTAL_HEIGHT = 3.6f;
	static constexpr float PORTAL_X = 3.0f;

	bool DemoIg00_Scene::init()
	{
		RenderState::enableBlending();
		RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);
		RenderState::enableMultisampleAntiAliasing();
		RenderState::setBackgroundColor(0.45f, 0.70f, 0.95f, 1.0f);

		createCamera();

		m_ground.create(80.0f, GROUND_HEIGHT);
		m_ground.setPosition({ 0.0f, GROUND_CENTER_Y });
		m_ground.setColor({ 0.25f, 0.55f, 0.25f, 1.0f });
		m_groundTopY = GROUND_CENTER_Y + GROUND_HEIGHT * 0.5f;

		m_portalX = PORTAL_X;
		m_portal.create(PORTAL_WIDTH, PORTAL_HEIGHT);
		m_portal.setPosition({ m_portalX, m_groundTopY + PORTAL_HEIGHT * 0.5f });
		m_portal.setColor({ 0.15f, 0.12f, 0.28f, 0.92f });

		const glm::vec2 playerSize = { PLAYER_WIDTH, PLAYER_HEIGHT };
		m_player.create({ 0.0f, m_groundTopY + playerSize.y * 0.5f }, playerSize);
		updateCamera();

		auto dots = std::make_unique<SwimmingDotsRenderer>();
		if (!dots->init())
		{
			return false;
		}
		m_dotsRenderer = std::move(dots);

		auto functions = std::make_unique<FunctionGraphsRenderer>();
		if (!functions->init())
		{
			return false;
		}
		m_functionsRenderer = std::move(functions);

		m_activeRenderer = m_dotsRenderer.get();

		return true;
	}

	void DemoIg00_Scene::updatePortalTransition()
	{
		if (m_dotsRenderer == nullptr || m_functionsRenderer == nullptr)
		{
			return;
		}

		// Left of portal = dots, right of portal = function graphs
		if (m_player.getPosition().x >= m_portalX)
		{
			m_activeRenderer = m_functionsRenderer.get();
		}
		else
		{
			m_activeRenderer = m_dotsRenderer.get();
		}
	}

	void DemoIg00_Scene::update(double deltaTime)
	{
		const float dt = static_cast<float>(deltaTime);
		m_player.update(dt, m_groundTopY);
		updatePortalTransition();
		updateCamera();
		if (m_activeRenderer != nullptr)
		{
			m_activeRenderer->update(dt, m_player.getVisualState());
		}
	}

	void DemoIg00_Scene::render() const
	{
		Renderer2DSystem::beginFrame();
		RenderCommands::clear();
		m_ground.render();
		Renderer2DSystem::endFrame();

		// Player first so the portal draws on top (player goes "behind" the door)
		if (m_activeRenderer != nullptr)
		{
			m_activeRenderer->render(m_player.getVisualState());
		}

		Renderer2DSystem::beginFrame();
		m_portal.render();
		Renderer2DSystem::endFrame();
	}

	void DemoIg00_Scene::exit()
	{
		if (m_dotsRenderer != nullptr)
		{
			m_dotsRenderer->destroy();
			m_dotsRenderer.reset();
		}
		if (m_functionsRenderer != nullptr)
		{
			m_functionsRenderer->destroy();
			m_functionsRenderer.reset();
		}
		m_activeRenderer = nullptr;
		m_portal.destroy();
		m_ground.destroy();
	}

	bool DemoIg00_Scene::onKeyPressed(const KeyPressedEvent& event)
	{
		if (event.getKeyCode() == KeyCode::KEY_ESCAPE && m_application != nullptr)
		{
			m_application->stopRunning();
			return true;
		}
		return false;
	}

	void DemoIg00_Scene::createCamera()
	{
		m_camera = std::make_shared<Camera2D>();
		m_camera->create(CAMERA_WORLD_WIDTH, CAMERA_WORLD_HEIGHT);
		Renderer2DSystem::setCamera(m_camera);
	}

	void DemoIg00_Scene::updateCamera()
	{
		if (m_camera == nullptr)
		{
			return;
		}
		const glm::vec2 playerPos = m_player.getPosition();
		// Player a bit above vertical center
		const float verticalBias = -CAMERA_WORLD_HEIGHT * 0.04f;
		m_camera->setPosition({ playerPos.x, playerPos.y + verticalBias });
	}

} // namespace Demo
