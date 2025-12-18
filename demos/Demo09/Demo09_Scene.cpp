#include "Demo09_Scene.h"

#include "Demo09_GUIWindow.h"
#include "TransformComponent2D.h"
#include "TransformSystem2D.h"
#include "SpriteComponent.h"
#include "RectangleGeometryComponent.h"
#include "PolygonGeometryComponent.h"
#include "LineGeometryComponent.h"
#include "TriangleGeometryComponent.h"
#include "CircleGeometryComponent.h"
#include "SolidColorMaterialComponent.h"
#include "LineComponent.h"
#include "CameraComponent2D.h"
#include "CameraSystem2D.h"
#include "Image.h"
#include "Renderer2DSubsystem.h"
#include "PostProcessor.h"
#include "ShaderPreprocessor.h"
#include "PekanEngine.h"
#include "PekanLogger.h"

using namespace Pekan::Renderer2D;
using namespace Pekan::Graphics;
using namespace Pekan;

#define PI 3.14159265359f

#define TURKEY_IMAGE_FILEPATH "resources/Turkey_animation_without_shadow.png"
#define BULL_IMAGE_FILEPATH "resources/Bull_animation_without_shadow.png"
#define PIG_IMAGE_FILEPATH "resources/Piglet_animation_without_shadow.png"

#define POST_PROCESSING_SHADER_FILEPATH_PKSHAD "Shaders/PostProcessingShader.pkshad"
#define POST_PROCESSING_SHADER_FILEPATH_GLSL "Shaders/PostProcessingShader.glsl"

namespace Demo
{

	constexpr float CAMERA_SCALE = 10.0f;
	constexpr glm::vec2 TURKEY_INITIAL_POSITION = glm::vec2(-CAMERA_SCALE / 2.0f, 0.0f);
	constexpr glm::vec2 TURKEY_SIZE = glm::vec2(1.0f, 1.0f);
	constexpr glm::vec2 BULL_INITIAL_POSITION = glm::vec2(CAMERA_SCALE / 2.0f, 0.0f);
	constexpr glm::vec2 BULL_SIZE = glm::vec2(2.0f, 2.0f);
	constexpr glm::vec2 PIG_SIZE = glm::vec2(1.0f, 1.0f);
	constexpr glm::vec2 RECTANGLE_INITIAL_POSITION = glm::vec2(-3.0f, -2.0f);
	constexpr glm::vec4 RECTANGLE_INITIAL_COLOR = glm::vec4(0.3f, 0.8f, 0.3f, 1.0f);
	constexpr glm::vec2 TRIANGLE_INITIAL_POSITION = glm::vec2(-1.0f, 3.0f);
	constexpr glm::vec4 TRIANGLE_INITIAL_COLOR = glm::vec4(0.8f, 0.3f, 0.3f, 1.0f);
	constexpr glm::vec2 POLYGON1_INITIAL_POSITION = glm::vec2(-4.0f, 3.0f);
	constexpr glm::vec4 POLYGON1_INITIAL_COLOR = glm::vec4(0.6f, 0.6f, 0.2f, 1.0f);
	constexpr glm::vec2 POLYGON2_INITIAL_POSITION = glm::vec2(4.0f, -3.0f);
	constexpr glm::vec4 POLYGON2_INITIAL_COLOR = glm::vec4(0.2f, 0.6f, 0.6f, 1.0f);
	constexpr glm::vec2 CIRCLE_INITIAL_POSITION = glm::vec2(2.0f, 2.0f);
	constexpr glm::vec4 CIRCLE_INITIAL_COLOR = glm::vec4(0.3f, 0.3f, 0.8f, 1.0f);
	constexpr glm::vec4 LINE1_INITIAL_COLOR = glm::vec4(0.8f, 0.8f, 0.2f, 1.0f);
	constexpr glm::vec4 LINE2_INITIAL_COLOR = glm::vec4(0.4f, 0.7f, 0.3f, 1.0f);

	static float osc(float t)
	{
		return cos(t + 1.0f) / 2.0f;
	}
	static float osc(float t, float a, float b)
	{
		return a + (b - a) * osc(t);
	}

	constexpr float kernelIdentity[9] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};

	static void lerpKernels(const float* a, const float* b, float* out, float w)
	{
		for (int i = 0; i < 9; i++)
		{
			out[i] = (1.0f - w) * a[i] + w * b[i];
		}
	}

	static void getKernelSharpen(float* kernel, float t)
	{
		float w = 0.5f * (sinf(t * 2.0f) + 1.0f);
		static constexpr float base[9] = {
			 0.0f, -1.0f,  0.0f,
			-1.0f,  5.0f, -1.0f,
			 0.0f, -1.0f,  0.0f
		};
		lerpKernels(kernelIdentity, base, kernel, w);
	}

	static void getKernelBlur(float* kernel, float t)
	{
		float w = 0.5f * (sinf(t * 2.0f) + 1.0f);
		static constexpr float base[9] = {
			1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f,
			1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f,
			1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f
		};
		lerpKernels(kernelIdentity, base, kernel, w);
	}

	static void getKernelEdgeDetection(float* kernel, float t)
	{
		float w = 0.5f * (sinf(t * 2.0f) + 1.0f);
		static constexpr float base[9] = {
			 1.0f,  1.0f,  1.0f,
			 1.0f, -8.0f,  1.0f,
			 1.0f,  1.0f,  1.0f
		};
		lerpKernels(kernelIdentity, base, kernel, w);
	}

	static void getKernelEmboss(float* kernel, float t)
	{
		float w = 0.5f * (sinf(t * 2.0f) + 1.0f);
		static constexpr float base[9] = {
			-2.0f, -1.0f,  0.0f,
			-1.0f,  1.0f,  1.0f,
			 0.0f,  1.0f,  2.0f
		};
		lerpKernels(kernelIdentity, base, kernel, w);
	}

	bool Demo09_Scene::_init()
	{
		if (m_guiWindow == nullptr)
		{
			PK_LOG_ERROR("Cannot initialize Demo09_Scene because there is no GUI window attached.", "Demo09");
			return false;
		}

		RenderState::enableMultisampleAntiAliasing();
		RenderState::enableBlending();
		RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

		createTurkey();
		createBull();
		createPig();
		createRectangle();
		createTriangle();
		createPolygon1();
		createPolygon2();
		createCircle();
		createCircle2();
		createLine1();
		createLine2();
		createLine3();
		createCamera();

		if (!initPps())
		{
			return false;
		}

		return true;
	}

	void Demo09_Scene::_exit()
	{
		destroyEntity(m_turkey);
		destroyEntity(m_bull);
		destroyEntity(m_pig);
		destroyEntity(m_rectangle);
		destroyEntity(m_triangle);
		destroyEntity(m_polygon1);
		destroyEntity(m_polygon2);
		destroyEntity(m_circle);
		destroyEntity(m_circle2);
		destroyEntity(m_line1);
		destroyEntity(m_line2);
		destroyEntity(m_line3);
		destroyEntity(m_camera);
	}

	void Demo09_Scene::update(double deltaTime)
	{
		entt::registry& registry = getRegistry();

		static const float leftEdge = -registry.get<CameraComponent2D>(m_camera).size.x / 2.0f;
		static const float rightEdge = registry.get<CameraComponent2D>(m_camera).size.x / 2.0f;

		// Move turkey
		{
			static glm::vec2 velocity = glm::vec2(0.02f, 0.0f);

			TransformComponent2D& transform = registry.get<TransformComponent2D>(m_turkey);
			transform.move(velocity);

			const float posX = transform.position.x;
			// Reverse velocity if entity reaches left or right edge of the camera view
			if (posX < leftEdge || posX > rightEdge)
			{
				velocity.x = -velocity.x;
			}
		}
		// Move bull
		{
			static glm::vec2 velocity = glm::vec2(-0.02f, 0.0f);

			TransformComponent2D& transform = registry.get<TransformComponent2D>(m_bull);
			transform.move(velocity);

			const float posX = transform.position.x;
			// Reverse velocity if entity reaches left or right edge of the camera view
			if (posX < leftEdge || posX > rightEdge)
			{
				velocity.x = -velocity.x;
			}
		}

		// Change color of rectangle over time
		{
			SolidColorMaterialComponent& material = registry.get<SolidColorMaterialComponent>(m_rectangle);
			material.color.r = osc(t, 0.3f, 0.8f);
			material.color.g = osc(t + 2.0f, 0.3f, 0.8f);
			material.color.b = osc(t + 4.0f, 0.3f, 0.8f);
		}
		// Move rectangle with mouse and rotate it over time
		{
			TransformComponent2D& rectangleTransform = registry.get<TransformComponent2D>(m_rectangle);

			// Move rectangle to follow mouse position
			const glm::vec2 mousePos = Renderer2DSubsystem::getMousePosition_ECS(registry);
			rectangleTransform.position = mousePos;

			rectangleTransform.rotation = osc(t * 2.0f, -0.1f, 0.1f);
		}
		// Change color of triangle over time
		{
			SolidColorMaterialComponent& material = registry.get<SolidColorMaterialComponent>(m_triangle);
			material.color.r = osc(t * 1.5f + 1.0f, 0.5f, 1.0f);
			material.color.g = osc(t * 0.9f + 3.0f, 0.2f, 0.5f);
			material.color.b = osc(t * 1.2f + 5.0f, 0.2f, 0.4f);
		}
		// Move triangle's vertices, rotate and stretch the whole triangle over time
		{
			TriangleGeometryComponent& triangleGeometry = registry.get<TriangleGeometryComponent>(m_triangle);
			// Make each vertex oscillate slightly
			triangleGeometry.pointA = glm::vec2(0.0f + osc(t * 2.0f, -0.1f, 0.1f), 1.0f + osc(t * 1.5f, -0.05f, 0.05f));
			triangleGeometry.pointB = glm::vec2(-1.0f + osc(t * 1.8f, -0.08f, 0.08f), -0.5f + osc(t * 2.2f, -0.06f, 0.06f));
			triangleGeometry.pointC = glm::vec2(1.0f + osc(t * 2.3f, -0.09f, 0.09f), -0.5f + osc(t * 1.7f, -0.07f, 0.07f));

			// Rotate the whole triangle
			TransformComponent2D& triangleTransform = registry.get<TransformComponent2D>(m_triangle);
			triangleTransform.rotation = osc(t * 1.3f, -0.3f, 0.3f);
			// Stretch and squash the triangle
			const float scaleX = osc(t * 1.6f, 0.8f, 1.2f);
			const float scaleY = osc(t * 1.9f, 0.9f, 1.3f);
			triangleTransform.scaleFactor = glm::vec2(scaleX, scaleY);
		}
		// Change color of polygon 1 over time
		{
			SolidColorMaterialComponent& material = registry.get<SolidColorMaterialComponent>(m_polygon1);
			material.color.r = osc(t * 1.4f + 0.0f, 0.3f, 0.9f);
			material.color.g = osc(t * 1.8f + 2.0f, 0.3f, 0.9f);
			material.color.b = osc(t * 1.1f + 4.0f, 0.3f, 0.9f);
		}
		// Oscillate vertex positions of polygon 1 and rotate it over time
		{

			PolygonGeometryComponent& geometry = registry.get<PolygonGeometryComponent>(m_polygon1);
			geometry.vertexPositions.resize(8);
			// Base is an octagon-like shape; perturb each vertex slightly with different oscillation parameters
			for (int i = 0; i < 8; ++i)
			{
				const float angle = (PI * 2.0f * i) / 8.0f;
				const float baseR = 1.2f + 0.2f * ((i % 2) ? 1.0f : -0.6f);
				// Each vertex gets unique oscillation parameters (frequency and amplitude)
				const float freq = 1.0f + 0.23f * i;
				const float amp = 0.04f + 0.01f * i;
				const float r = baseR + osc(t * freq, -amp, amp);
				geometry.vertexPositions[i] = glm::vec2(r * cos(angle), r * sin(angle));
			}
			TransformComponent2D& transform = registry.get<TransformComponent2D>(m_polygon1);
			transform.rotation += 0.25f * float(deltaTime);
		}
		// Change color of polygon 2 over time
		{
			SolidColorMaterialComponent& material = registry.get<SolidColorMaterialComponent>(m_polygon2);
			material.color.r = osc(t * 0.9f + 1.0f, 0.2f, 0.8f);
			material.color.g = osc(t * 1.3f + 3.0f, 0.2f, 0.8f);
			material.color.b = osc(t * 1.6f + 5.0f, 0.2f, 0.8f);
		}
		// Oscillate vertex positions of polygon 2 and scale it over time
		{

			PolygonGeometryComponent& geometry = registry.get<PolygonGeometryComponent>(m_polygon2);
			geometry.vertexPositions.resize(11);
			// Make a concave 11-gon: alternate inner and outer radius to form a star-like concavity
			for (int i = 0; i < 11; ++i)
			{
				const float angle = (PI * 2.0f * i) / 11.0f;
				const float outer = 1.4f;
				const float inner = 0.6f;
				const float baseR = (i % 2 == 0) ? outer : inner;
				// Each vertex gets unique oscillation parameters
				const float freq = 1.2f + 0.17f * i;
				const float amp = 0.05f + 0.008f * i;
				const float r = baseR + osc(t * freq, -amp, amp);
				geometry.vertexPositions[i] = glm::vec2(r * cos(angle), r * sin(angle));
			}
			TransformComponent2D& transform = registry.get<TransformComponent2D>(m_polygon2);
			const float scaleX = osc(t * 1.7f, 0.8f, 1.25f);
			const float scaleY = osc(t * 2.2f, 0.8f, 1.25f);
			transform.scaleFactor = glm::vec2(scaleX, scaleY);
		}
		// Change color of circle over time
		{
			SolidColorMaterialComponent& material = registry.get<SolidColorMaterialComponent>(m_circle);
			material.color.r = osc(t * 2.2f + 4.0f, 0.5f, 0.8f);
			material.color.g = osc(t * 0.7f + 2.0f, 0.3f, 0.6f);
			material.color.b = osc(t * 1.0f + 0.0f, 0.1f, 0.9f);
		}
		// Move circle left and right and scale it over time
		{
			TransformComponent2D& circleTransform = registry.get<TransformComponent2D>(m_circle);
			circleTransform.position.x = osc(t * 2.1f, 1.5f, 2.5f);
			const float scaleX = osc(t * 3.2f, 0.75f, 1.25f);
			const float scaleY = osc(t * 2.3f, 0.85f, 1.5f);
			circleTransform.scaleFactor = glm::vec2(scaleX, scaleY);
		}
		// Change color of line 1 over time
		{
			SolidColorMaterialComponent& material = registry.get<SolidColorMaterialComponent>(m_line1);
			material.color.r = osc(t * 1.7f + 3.0f, 0.4f, 0.9f);
			material.color.g = osc(t * 2.5f + 1.0f, 0.4f, 0.9f);
			material.color.b = osc(t * 1.3f + 5.0f, 0.4f, 0.9f);
		}
		// Update line 1's endpoints so that they always connect rectangle's center with circle's center
		{
			LineGeometryComponent& geometry = registry.get<LineGeometryComponent>(m_line1);
			const TransformComponent2D& rectangleTransform = registry.get<TransformComponent2D>(m_rectangle);
			const TransformComponent2D& circleTransform = registry.get<TransformComponent2D>(m_circle);
			geometry.pointA = rectangleTransform.position;
			geometry.pointB = circleTransform.position;
		}
		// Update line 2's endpoints so that they always connect polygon 1's center with polygon 2's center
		// and change its color over time
		{
			LineComponent& line = registry.get<LineComponent>(m_line2);
			const TransformComponent2D& polygon1Transform = registry.get<TransformComponent2D>(m_polygon1);
			const TransformComponent2D& polygon2Transform = registry.get<TransformComponent2D>(m_polygon2);
			line.pointA = polygon1Transform.position;
			line.pointB = polygon2Transform.position;

			line.color.r = osc(t * 1.2f + 4.0f, 0.4f, 0.9f);
			line.color.g = osc(t * 3.5f + 2.0f, 0.2f, 0.9f);
			line.color.b = osc(t * 1.9f + 3.5f, 0.4f, 0.9f);
		}

		updatePps();

		t += static_cast<float>(deltaTime);
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

	void Demo09_Scene::createPig()
	{
		m_pig = createEntity();

		// Add sprite component to pig entity
		{
			SpriteComponent sprite;
			sprite.width = PIG_SIZE.x;
			sprite.height = PIG_SIZE.y;
			// Set sprite texture
			{
				Texture2D_Ptr texture = std::make_shared<Pekan::Graphics::Texture2D>();
				const Image image(PIG_IMAGE_FILEPATH);
				texture->create(image);
				sprite.texture = texture;
			}
			// Set sprite texture coordinates
			// to use only one of the frames of the pig animation spritesheet
			{
				sprite.textureCoordinatesMin = glm::vec2(0.0f, 1.0f / 8.0f);
				sprite.textureCoordinatesMax = glm::vec2(1.0f / 6.0f, 2.0f / 8.0f);
			}
			getRegistry().emplace<SpriteComponent>(m_pig, sprite);
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

	void Demo09_Scene::createTriangle()
	{
		m_triangle = createEntity();
		// Add transform component to triangle entity
		getRegistry().emplace<TransformComponent2D>(m_triangle, TRIANGLE_INITIAL_POSITION);
		// Add triangle geometry component to triangle entity
		TriangleGeometryComponent geometry;
		geometry.pointA = glm::vec2(0.0f, 1.0f);
		geometry.pointB = glm::vec2(-1.0f, -0.5f);
		geometry.pointC = glm::vec2(1.0f, -0.5f);
		getRegistry().emplace<TriangleGeometryComponent>(m_triangle, std::move(geometry));
		// Add solid color material component to triangle entity
		getRegistry().emplace<SolidColorMaterialComponent>(m_triangle, TRIANGLE_INITIAL_COLOR);
	}

	void Demo09_Scene::createPolygon1()
	{
		m_polygon1 = createEntity();
		// Add transform component to polygon 1's entity
		getRegistry().emplace<TransformComponent2D>(m_polygon1, POLYGON1_INITIAL_POSITION);
		// Add polygon geometry component to polygon 1's entity
		PolygonGeometryComponent geometry;
		geometry.vertexPositions.resize(8);
		for (int i = 0; i < 8; ++i)
		{
			const float angle = (PI * 2.0f * i) / 8.0f;
			const float r = 1.0f + 0.2f * ((i % 2) ? 1.0f : 0.0f);
			geometry.vertexPositions[i] = glm::vec2(r * cos(angle), r * sin(angle));
		}
		getRegistry().emplace<PolygonGeometryComponent>(m_polygon1, std::move(geometry));
		// Add solid color material component to polygon 1's entity
		getRegistry().emplace<SolidColorMaterialComponent>(m_polygon1, POLYGON1_INITIAL_COLOR);
	}

	void Demo09_Scene::createPolygon2()
	{
		m_polygon2 = createEntity();
		// Add transform component to polygon 2's entity
		getRegistry().emplace<TransformComponent2D>(m_polygon2, POLYGON2_INITIAL_POSITION);
		// Add polygon geometry component to polygon 2's entity
		PolygonGeometryComponent geometry;
		geometry.vertexPositions.resize(11);
		for (int i = 0; i < 11; ++i)
		{
			const float angle = (PI * 2.0f * i) / 11.0f;
			const float outer = 1.2f;
			const float inner = 0.5f;
			const float r = (i % 2 == 0) ? outer : inner;
			geometry.vertexPositions[i] = glm::vec2(r * cos(angle), r * sin(angle));
		}
		getRegistry().emplace<PolygonGeometryComponent>(m_polygon2, std::move(geometry));
		// Add solid color material component to polygon 1's entity
		getRegistry().emplace<SolidColorMaterialComponent>(m_polygon2, POLYGON2_INITIAL_COLOR);
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

	void Demo09_Scene::createCircle2()
	{
		m_circle2 = createEntity();

		// Add circle geometry component to circle entity
		getRegistry().emplace<CircleGeometryComponent>(m_circle2, 0.3f, 64);
		// Add solid color material component to circle entity
		getRegistry().emplace<SolidColorMaterialComponent>(m_circle2, glm::vec4{0.3f, 0.8f, 0.2f, 1.0f});
	}

	void Demo09_Scene::createLine1()
	{
		m_line1 = createEntity();
		// Add transform component to line 1's entity
		getRegistry().emplace<TransformComponent2D>(m_line1);
		// Add line geometry component to line 1's entity
		getRegistry().emplace<LineGeometryComponent>(m_line1, RECTANGLE_INITIAL_POSITION, CIRCLE_INITIAL_POSITION);
		// Add solid color material component to line 1's entity
		getRegistry().emplace<SolidColorMaterialComponent>(m_line1, LINE1_INITIAL_COLOR);
	}

	void Demo09_Scene::createLine2()
	{
		m_line2 = createEntity();
		// Add transform component to line 2's entity
		getRegistry().emplace<TransformComponent2D>(m_line2);
		// Add line component to line 2's entity
		LineComponent lineComponent;
		lineComponent.pointA = POLYGON1_INITIAL_POSITION;
		lineComponent.pointB = POLYGON2_INITIAL_POSITION;
		lineComponent.color = LINE2_INITIAL_COLOR;
		getRegistry().emplace<LineComponent>(m_line2, lineComponent);
	}

	void Demo09_Scene::createLine3()
	{
		m_line3 = createEntity();

		// Add line component to line 3's entity
		LineComponent lineComponent;
		lineComponent.pointA = glm::vec2(-CAMERA_SCALE / 30.0f, -CAMERA_SCALE / 30.0f);
		lineComponent.pointB = glm::vec2(CAMERA_SCALE / 30.0f, CAMERA_SCALE / 30.0f);
		lineComponent.color = glm::vec4(0.9f, 0.4f, 0.2f, 1.0f);
		getRegistry().emplace<LineComponent>(m_line3, lineComponent);
	}

	void Demo09_Scene::createCamera()
	{
		m_camera = createEntity();
		// Add camera component to camera entity
		CameraComponent2D cameraComponent;
		cameraComponent.setHeight(CAMERA_SCALE);
		getRegistry().emplace<CameraComponent2D>(m_camera, cameraComponent);
	}

	bool Demo09_Scene::initPps()
	{
		ShaderPreprocessor::preprocess
		(
			POST_PROCESSING_SHADER_FILEPATH_PKSHAD,
			{
				{ "NEIGHBORS_SAMPLE_OFFSET_INVERSE_X", std::to_string(PekanEngine::getWindow().getSize().x / 3) },
				{ "NEIGHBORS_SAMPLE_OFFSET_INVERSE_Y", std::to_string(PekanEngine::getWindow().getSize().y / 3) }
			}
		);

		PostProcessor::setPostProcessingShader(POST_PROCESSING_SHADER_FILEPATH_GLSL);

		return true;
	}

	void Demo09_Scene::updatePps()
	{
		Shader* postProcessorShader = PostProcessor::getShader();
		PK_ASSERT_QUICK(postProcessorShader != nullptr);
		static float kernel[9];
		const int ppsIndex = m_guiWindow->getPpsIndex();
		switch (ppsIndex)
		{
			case 0: postProcessorShader->setUniform1fv("kernel", 9, kernelIdentity); return;
			case 1: getKernelSharpen(kernel, t); break;
			case 2: getKernelBlur(kernel, t); break;
			case 3: getKernelEdgeDetection(kernel, t); break;
			case 4: getKernelEmboss(kernel, t); break;
			default: PK_LOG_ERROR("Invalid post-processing shader selected in GUI.", "Demo06");
		}
		postProcessorShader->setUniform1fv("kernel", 9, kernel);
	}

} // namespace Demo
