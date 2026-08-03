#include "DemoIg00_Scene.h"

#include "PekanApplication.h"
#include "Events/KeyEvents.h"
#include "RenderCommands.h"
#include "RenderState.h"
#include "Renderer2DSystem.h"

using Pekan::KeyPressedEvent;
using Pekan::KeyCode;

using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;

namespace Demo
{

	// World height in units (width follows window aspect)
	static constexpr float CAMERA_HEIGHT = 10.0f;

	bool DemoIg00_Scene::init()
	{
		RenderState::enableBlending();
		RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);
		RenderState::enableMultisampleAntiAliasing();
		RenderState::setBackgroundColor(0.45f, 0.70f, 0.95f, 1.0f);

		createCamera();

		// Placeholder ground platform
		m_ground.create(24.0f, 1.0f);
		m_ground.setPosition({ 0.0f, -4.0f });
		m_ground.setColor({ 0.25f, 0.55f, 0.25f, 1.0f });

		// Placeholder player
		m_player.create(0.8f, 1.2f);
		m_player.setPosition({ 0.0f, -2.9f });
		m_player.setColor({ 0.90f, 0.35f, 0.25f, 1.0f });

		return true;
	}

	void DemoIg00_Scene::update(double deltaTime)
	{
		// Ready for platformer logic:
		// PekanEngine::isKeyPressed(KeyCode::KEY_A / KEY_D / KEY_LEFT / KEY_RIGHT) — move
		// PekanEngine::isKeyPressed(KeyCode::KEY_SPACE / KEY_W / KEY_UP) — jump
		(void)deltaTime;
	}

	void DemoIg00_Scene::render() const
	{
		Renderer2DSystem::beginFrame();
		RenderCommands::clear();

		m_ground.render();
		m_player.render();

		Renderer2DSystem::endFrame();
	}

	void DemoIg00_Scene::exit()
	{
		m_player.destroy();
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
		m_camera->create(CAMERA_HEIGHT);
		Renderer2DSystem::setCamera(m_camera);
	}

} // namespace Demo
