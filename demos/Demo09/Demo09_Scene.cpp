#include "Demo09_Scene.h"

#include "TransformComponent2D.h"
#include "TransformSystem2D.h"
#include "PekanLogger.h"
#include "SpriteComponent.h"
#include "Image.h"
#include "Renderer2DSystem.h"
#include "PekanTools.h"

using namespace Pekan::Renderer2D;
using namespace Pekan::Graphics;
using namespace Pekan::Tools;

#define TURKEY_IMAGE_FILEPATH "resources/Turkey_animation_without_shadow.png"
#define BULL_IMAGE_FILEPATH "resources/Bull_animation_without_shadow.png"

namespace Demo
{

	constexpr float CAMERA_SCALE = 10.0f;
	constexpr glm::vec2 TURKEY_INITIAL_POSITION = glm::vec2(-CAMERA_SCALE / 2.0f, 0.0f);
	constexpr glm::vec2 BULL_INITIAL_POSITION = glm::vec2(CAMERA_SCALE / 2.0f, 0.0f);

	bool Demo09_Scene::init()
	{
		RenderState::enableMultisampleAntiAliasing();
		RenderState::enableBlending();
		RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

		createTurkey();
		createBull();
		createCamera();

		return true;
	}

	void Demo09_Scene::update(double deltaTime)
	{
		entt::registry& registry = getRegistry();

		// Move turkey
		TransformSystem2D::move(registry, m_turkey, glm::vec2(0.02f, 0.0f));
		// Move bull
		TransformSystem2D::move(registry, m_bull, glm::vec2(-0.02f, 0.0f));
	}

	void Demo09_Scene::exit()
	{
		destroyEntity(m_turkey);
		m_camera->destroy();
	}

	void Demo09_Scene::createTurkey()
	{
		m_turkey = createEntity();

		// Add transform component to turkey entity
		getRegistry().emplace<TransformComponent2D>(m_turkey, TURKEY_INITIAL_POSITION);

		// Add sprite component to turkey entity
		{
			SpriteComponent sprite;
			sprite.width = 1.0f;
			sprite.height = 1.0f;
			// Set sprite texture
			{
				Texture2D_Ptr texture = std::make_shared<Pekan::Graphics::Texture2D>();
				const Image image(TURKEY_IMAGE_FILEPATH);
				texture->create(image);
				sprite.texture = texture;
			}
			// Set sprite texture coordinates
			// to use only one of the frames of the turkey animation spritesheet
			{
				sprite.textureCoordinatesMin = glm::vec2(0.0f, 0.0f);
				sprite.textureCoordinatesMax = glm::vec2(1.0f / 6.0f, 1.0f / 8.0f);
			}
			getRegistry().emplace<SpriteComponent>(m_turkey, sprite);
		}
	}

	void Demo09_Scene::createBull()
	{
		m_bull = createEntity();

		// Add transform component to bull entity
		getRegistry().emplace<TransformComponent2D>(m_bull, BULL_INITIAL_POSITION);

		// Add sprite component to bull entity
		{
			SpriteComponent sprite;
			sprite.width = 2.0f;
			sprite.height = 2.0f;
			// Set sprite texture
			{
				Texture2D_Ptr texture = std::make_shared<Pekan::Graphics::Texture2D>();
				const Image image(BULL_IMAGE_FILEPATH);
				texture->create(image);
				sprite.texture = texture;
			}
			// Set sprite texture coordinates
			// to use only one of the frames of the bull animation spritesheet
			{
				sprite.textureCoordinatesMin = glm::vec2(0.0f, 1.0f / 8.0f);
				sprite.textureCoordinatesMax = glm::vec2(1.0f / 6.0f, 2.0f / 8.0f);
			}
			getRegistry().emplace<SpriteComponent>(m_bull, sprite);
		}
	}

	void Demo09_Scene::createCamera()
	{
		m_camera = std::make_shared<Camera2D>();
		m_camera->create(CAMERA_SCALE);
		Renderer2DSystem::setCamera(m_camera);
		PekanTools::enableCameraController2D(m_camera);
		PekanTools::setCameraController2DZoomSpeed(1.1f);
	}

} // namespace Demo
