#include "Demo09_Scene.h"

#include "TransformComponent2D.h"
#include "TransformSystem2D.h"
#include "PekanLogger.h"
#include "SpriteComponent.h"
#include "RectangleGeometryComponent.h"
#include "SolidColorMaterialComponent.h"
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
	constexpr glm::vec2 GROUND_POSITION = glm::vec2(-3.0f, -2.0f);
	constexpr glm::vec4 GROUND_COLOR = glm::vec4(0.3f, 0.8f, 0.3f, 1.0f);

	static float osc(float t)
	{
		return cos(t + 1.0f) / 2.0f;
	}
	static float osc(float t, float a, float b)
	{
		return a + (b - a) * osc(t);
	}

	bool Demo09_Scene::init()
	{
		RenderState::enableMultisampleAntiAliasing();
		RenderState::enableBlending();
		RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

		createTurkey();
		createBull();
		createGround();
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

		// Change color of ground over time
		{
			SolidColorMaterialComponent& groundMaterial = registry.get<SolidColorMaterialComponent>(m_ground);
			groundMaterial.color.r = osc(t, 0.3f, 0.8f);
			groundMaterial.color.g = osc(t + 2.0f, 0.3f, 0.8f);
			groundMaterial.color.b = osc(t + 4.0f, 0.3f, 0.8f);
		}
		// Move ground up and down and rotate it over time
		{
			TransformComponent2D& groundTransform = registry.get<TransformComponent2D>(m_ground);
			groundTransform.position.y = osc(t * 3.0f, -1.5f, -2.5f);
			groundTransform.rotation = osc(t * 2.0f, -0.2f, 0.2f);
		}

		t += static_cast<float>(deltaTime);
	}

	void Demo09_Scene::exit()
	{
		destroyEntity(m_turkey);
		destroyEntity(m_bull);
		destroyEntity(m_ground);
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

	void Demo09_Scene::createGround()
	{
		m_ground = createEntity();

		// Add transform component to ground entity
		getRegistry().emplace<TransformComponent2D>(m_ground, GROUND_POSITION);

		// Add rectangle geometry component to ground entity
		getRegistry().emplace<RectangleGeometryComponent>(m_ground, 6.0f, 1.0f);

		// Add solid color material component to ground entity
		getRegistry().emplace<SolidColorMaterialComponent>(m_ground, GROUND_COLOR);
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
