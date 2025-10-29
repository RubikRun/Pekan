#include "Demo09_Scene.h"

#include "TransformComponent2D.h"
#include "TransformSystem2D.h"
#include "PekanLogger.h"
#include "SpriteComponent.h"
#include "RectangleGeometryComponent.h"
#include "CircleGeometryComponent.h"
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
	constexpr glm::vec2 TURKEY_SIZE = glm::vec2(1.0f, 1.0f);
	constexpr glm::vec2 BULL_INITIAL_POSITION = glm::vec2(CAMERA_SCALE / 2.0f, 0.0f);
	constexpr glm::vec2 BULL_SIZE = glm::vec2(2.0f, 2.0f);
	constexpr glm::vec2 RECTANGLE_INITIAL_POSITION = glm::vec2(-3.0f, -2.0f);
	constexpr glm::vec4 RECTANGLE_INITIAL_COLOR = glm::vec4(0.3f, 0.8f, 0.3f, 1.0f);
	constexpr glm::vec2 CIRCLE_INITIAL_POSITION = glm::vec2(2.0f, 2.0f);
	constexpr glm::vec4 CIRCLE_INITIAL_COLOR = glm::vec4(0.3f, 0.3f, 0.8f, 1.0f);

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
		createRectangle();
		createCircle();
		createCamera();

		return true;
	}

	void Demo09_Scene::update(double deltaTime)
	{
		entt::registry& registry = getRegistry();

		// Move turkey
		{
			static glm::vec2 velocity = glm::vec2(0.02f, 0.0f);
			TransformSystem2D::move(registry, m_turkey, velocity);

			const float posX = TransformSystem2D::getPosition(registry, m_turkey).x;
			// Reverse velocity if entity reaches left or right edge of the camera view
			PK_ASSERT_QUICK(m_camera != nullptr);
			if (posX < m_camera->getLeftEdgeInWorldSpace() || posX > m_camera->getRightEdgeInWorldSpace())
			{
				velocity.x = -velocity.x;
			}
		}
		// Move bull
		{
			static glm::vec2 velocity = glm::vec2(-0.02f, 0.0f);
			TransformSystem2D::move(registry, m_bull, velocity);

			const float posX = TransformSystem2D::getPosition(registry, m_bull).x;
			// Reverse velocity if entity reaches left or right edge of the camera view
			PK_ASSERT_QUICK(m_camera != nullptr);
			if (posX < m_camera->getLeftEdgeInWorldSpace() || posX > m_camera->getRightEdgeInWorldSpace())
			{
				velocity.x = -velocity.x;
			}
		}

		// Change color of rectangle over time
		{
			SolidColorMaterialComponent& rectangleMaterial = registry.get<SolidColorMaterialComponent>(m_rectangle);
			rectangleMaterial.color.r = osc(t, 0.3f, 0.8f);
			rectangleMaterial.color.g = osc(t + 2.0f, 0.3f, 0.8f);
			rectangleMaterial.color.b = osc(t + 4.0f, 0.3f, 0.8f);
		}
		// Move rectangle up and down and rotate it over time
		{
			TransformComponent2D& rectangleTransform = registry.get<TransformComponent2D>(m_rectangle);
			rectangleTransform.position.y = osc(t * 3.0f, -1.75f, -2.25f);
			rectangleTransform.rotation = osc(t * 2.0f, -0.1f, 0.1f);
		}
		// Change color of circle over time
		{
			SolidColorMaterialComponent& circleMaterial = registry.get<SolidColorMaterialComponent>(m_circle);
			circleMaterial.color.r = osc(t * 2.2f + 4.0f, 0.5f, 0.8f);
			circleMaterial.color.g = osc(t * 0.7f + 2.0f, 0.3f, 0.6f);
			circleMaterial.color.b = osc(t * 1.0f + 0.0f, 0.1f, 0.9f);
		}
		// Move circle left and right and scale it over time
		{
			TransformComponent2D& circleTransform = registry.get<TransformComponent2D>(m_circle);
			circleTransform.position.x = osc(t * 2.1f, 1.5f, 2.5f);
			const float scaleX = osc(t * 3.2f, 0.75f, 1.25f);
			const float scaleY = osc(t * 2.3f, 0.85f, 1.5f);
			circleTransform.scale = glm::vec2(scaleX, scaleY);
		}

		t += static_cast<float>(deltaTime);
	}

	void Demo09_Scene::exit()
	{
		destroyEntity(m_turkey);
		destroyEntity(m_bull);
		destroyEntity(m_rectangle);
		destroyEntity(m_circle);
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
			sprite.width = TURKEY_SIZE.x;
			sprite.height = TURKEY_SIZE.y;
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
			sprite.width = BULL_SIZE.x;
			sprite.height = BULL_SIZE.y;
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

	void Demo09_Scene::createRectangle()
	{
		m_rectangle = createEntity();
		// Add transform component to rectangle entity
		getRegistry().emplace<TransformComponent2D>(m_rectangle, RECTANGLE_INITIAL_POSITION);
		// Add rectangle geometry component to rectangle entity
		getRegistry().emplace<RectangleGeometryComponent>(m_rectangle, 6.0f, 1.0f);
		// Add solid color material component to rectangle entity
		getRegistry().emplace<SolidColorMaterialComponent>(m_rectangle, RECTANGLE_INITIAL_COLOR);
	}

	void Demo09_Scene::createCircle()
	{
		m_circle = createEntity();
		// Add transform component to circle entity
		getRegistry().emplace<TransformComponent2D>(m_circle, glm::vec2(2.0f, 0.0f));
		// Add circle geometry component to circle entity
		getRegistry().emplace<CircleGeometryComponent>(m_circle, 1.0f, 64);
		// Add solid color material component to circle entity
		getRegistry().emplace<SolidColorMaterialComponent>(m_circle, CIRCLE_INITIAL_COLOR);
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
