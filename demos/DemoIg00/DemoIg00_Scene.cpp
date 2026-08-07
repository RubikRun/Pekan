#include "DemoIg00_Scene.h"

#include "PekanApplication.h"
#include "Events/KeyEvents.h"
#include "RenderCommands.h"
#include "RenderState.h"
#include "Renderer2DSystem.h"
#include "ColorGridRenderer.h"

using Pekan::KeyPressedEvent;
using Pekan::KeyCode;

using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;

namespace Demo
{

	static constexpr float CAMERA_WORLD_HEIGHT = 7.0f;
	static constexpr float CAMERA_WORLD_WIDTH = CAMERA_WORLD_HEIGHT * (9.0f / 16.0f);
	static constexpr float GROUND_HEIGHT = 1.0f;
	static constexpr float GROUND_CENTER_Y = -2.5f;
	static constexpr float PLAYER_HEIGHT = 2.0f;
	static constexpr float PLAYER_WIDTH = PLAYER_HEIGHT * (232.0f / 439.0f);

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

		const glm::vec2 playerSize = { PLAYER_WIDTH, PLAYER_HEIGHT };
		m_player.create({ 0.0f, m_groundTopY + playerSize.y * 0.5f }, playerSize);
		updateCamera();

		auto renderer = std::make_unique<ColorGridRenderer>();
		if (!renderer->init())
		{
			return false;
		}
		m_playerRenderer = std::move(renderer);

		return true;
	}

	void DemoIg00_Scene::update(double deltaTime)
	{
		const float dt = static_cast<float>(deltaTime);
		m_player.update(dt, m_groundTopY);
		updateCamera();
		if (m_playerRenderer != nullptr)
		{
			m_playerRenderer->update(dt, m_player.getVisualState());
		}
	}

	void DemoIg00_Scene::render() const
	{
		Renderer2DSystem::beginFrame();
		RenderCommands::clear();
		m_ground.render();
		Renderer2DSystem::endFrame();

		if (m_playerRenderer != nullptr)
		{
			m_playerRenderer->render(m_player.getVisualState());
		}
	}

	void DemoIg00_Scene::exit()
	{
		if (m_playerRenderer != nullptr)
		{
			m_playerRenderer->destroy();
			m_playerRenderer.reset();
		}
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
		const float verticalBias = -CAMERA_WORLD_HEIGHT * 0.04f;
		m_camera->setPosition({ playerPos.x, playerPos.y + verticalBias });
	}

} // namespace Demo
