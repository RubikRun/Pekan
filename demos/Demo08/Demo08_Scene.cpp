#include "Demo08_Scene.h"

#include "Demo08_GUIWindow.h"
#include "PekanLogger.h"
#include "Utils/RandomizationUtils.h"
#include "RenderCommands.h"
#include "Renderer2DSubsystem.h"
#include "Image.h"
#include "CameraComponent2D.h"
#include "TransformComponent2D.h"
#include "SpriteComponent.h"
#include "RectangleGeometryComponent.h"
#include "SolidColorMaterialComponent.h"

#include <algorithm>

constexpr float ZOOM_SPEED = 1.1f;

using namespace Pekan;
using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;
using namespace Pekan::RandomizationUtils;

namespace Demo
{

	// Number of textures to be used for the sprites.
	// Textures will be loaded from .png files that are expected to be under "resources" directory
	// and named "00.png", "01.png", "02.png", etc.
	constexpr int TEXTURES_COUNT = 44;

	// Number of textures to be used for the animation sprite.
	// Textures will be loaded from .png files that are expected to be under "resources" directory
	// and named "anim00.png", "anim01.png", "anim02.png", etc.
	constexpr int ANIMATION_LENGTH = 10;

	// Size of animation sprite will be this factor times window's width.
	constexpr float ANIMATION_SIZE_FACTOR = 0.02f;

	// Loads textures to be used for the sprites
	//
	// NOTE: this works only for two-digit filenames, so it supports at most 100 textures (00 to 99).
	static void loadSpritesTextures(std::vector<Texture2D_Ptr>& textures)
	{
		textures.clear();
		textures.resize(TEXTURES_COUNT);
		for (size_t i = 0; i < TEXTURES_COUNT; i++)
		{
			// Generate image file's name
			std::string filename = "resources/";
			if (i < 10)
			{
				filename += "0";
			}
			filename += std::to_string(i) + ".png";
			// Load image
			Image image(filename.c_str());
			// Create texture
			textures[i] = std::make_shared<Texture2D>();
			textures[i]->create(image);
		}
	}

	// Loads textures to be used for the animation sprite.
	//
	// NOTE: this works only for two-digit filenames, so it supports at most 100 textures (00 to 99).
	static void loadAnimTextures(std::vector<Texture2D_Ptr>& textures)
	{
		textures.clear();
		textures.resize(ANIMATION_LENGTH);
		for (size_t i = 0; i < ANIMATION_LENGTH; i++)
		{
			// Generate image file's name
			std::string filename = "resources/anim";
			if (i < 10)
			{
				filename += "0";
			}
			filename += std::to_string(i) + ".png";
			// Load image
			Image image(filename.c_str());
			// Create texture
			textures[i] = std::make_shared<Texture2D>();
			textures[i]->create(image);
		}
	}

    bool Demo08_Scene::_init()
	{
		if (m_guiWindow == nullptr)
		{
			PK_LOG_ERROR("Cannot initialize Demo08_Scene because there is no GUI window attached.", "Demo08");
			return false;
		}

		RenderState::enableMultisampleAntiAliasing();

		// Enable and configure blending
		RenderState::enableBlending();
		RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

		m_windowSize = glm::vec2(PekanEngine::getWindow().getSize());

		m_spritesCount = m_guiWindow->getNumberOfSprites();
		m_spritesMaxCount = m_guiWindow->getMaxNumberOfSprites();

		createCamera();
		createSprites();
		createCenterSquare();
		createAnimSprite();

        return true;
	}

	void Demo08_Scene::_exit()
	{
		destroyEntity(m_animSprite);
		for (int i = 0; i < m_animTextures.size(); i++)
		{
			m_animTextures[i]->destroy();
		}
		destroyEntity(m_centerSquare);
		for (int i = 0; i < m_sprites.size(); i++)
		{
			destroyEntity(m_sprites[i]);
		}
		destroyEntity(m_camera);
	}

	void Demo08_Scene::update(double dt)
	{
		PK_ASSERT_QUICK(m_guiWindow != nullptr);
		m_spritesCount = m_guiWindow->getNumberOfSprites();

		updateSprites(float(dt));
		updateAnimSprite(float(dt));

		t += float(dt);
	}

	void Demo08_Scene::createCamera()
	{
		m_camera = createEntity();
		// Add camera component to camera entity
		CameraComponent2D cameraComponent;
		cameraComponent.setWidth(m_windowSize.x);
		cameraComponent.setHeight(m_windowSize.y);
		m_registry.emplace<CameraComponent2D>(m_camera, cameraComponent);
	}

	void Demo08_Scene::createSprites()
	{
		// Load textures
		std::vector<Texture2D_Ptr> textures;
		loadSpritesTextures(textures);

		// Define randomization parameters
		const float minDim = std::min(m_windowSize.x, m_windowSize.y);
		const glm::vec2 widthHeightRange =
		{
			minDim * 0.01f,
			minDim * 0.04f
		};
		const glm::vec2 positionMin = { -m_windowSize.x / 2.0f + widthHeightRange.y / 2.0f, -m_windowSize.y / 2.0f + widthHeightRange.y / 2.0f };
		const glm::vec2 positionMax = { m_windowSize.x / 2.0f - widthHeightRange.y / 2.0f, m_windowSize.y / 2.0f - widthHeightRange.y / 2.0f };

		// Generate sprites with random textures with a random size at a random position
		m_sprites.resize(m_spritesMaxCount);
		for (size_t i = 0; i < m_spritesMaxCount; i++)
		{
			m_sprites[i] = createEntity();

			// Add transform component to sprite entity
			{
				TransformComponent2D transform;
				transform.position = getRandomVec2(positionMin, positionMax);
				m_registry.emplace<TransformComponent2D>(m_sprites[i], transform);
			}
			// Add sprite component to sprite entity
			{
				SpriteComponent spriteComponent;
				spriteComponent.width = getRandomFloat(widthHeightRange.x, widthHeightRange.y);
				spriteComponent.height = getRandomFloat(widthHeightRange.x, widthHeightRange.y);
				const size_t textureIndex = getRandomInt(0, textures.size() - 1);
				spriteComponent.texture = textures[textureIndex];
				m_registry.emplace<SpriteComponent>(m_sprites[i], spriteComponent);
			}
		}

		// Initialize velocities list with { 0, 0 } velocity for each sprite
		m_spritesVelocities = std::vector<glm::vec2>(m_spritesMaxCount, { 0.0f, 0.0f });
	}

	void Demo08_Scene::createCenterSquare()
	{
		m_centerSquare = createEntity();
		// Add transform component to rectangle entity
		m_registry.emplace<TransformComponent2D>(m_centerSquare, glm::vec2{ 0.0f, 0.0f });
		// Add rectangle geometry component to rectangle entity
		m_registry.emplace<RectangleGeometryComponent>(m_centerSquare, 100.0f, 100.0f);
		// Add solid color material component to rectangle entity
		m_registry.emplace<SolidColorMaterialComponent>(m_centerSquare, glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
	}

	void Demo08_Scene::createAnimSprite()
	{
		// Load animation textures
		loadAnimTextures(m_animTextures);

		m_animSprite = createEntity();

		const float spriteSize = m_windowSize.x * ANIMATION_SIZE_FACTOR;
		m_currAnimTextureIndex = 0;
		// Add transform component to sprite entity
		{
			TransformComponent2D transform;
			transform.position = { -m_windowSize.x / 2.0f + spriteSize / 2.0f, m_windowSize.y / 2.0f - spriteSize / 2.0f };
			m_registry.emplace<TransformComponent2D>(m_animSprite, transform);
		}
		// Add sprite component to sprite entity
		{
			SpriteComponent spriteComponent;
			spriteComponent.width = spriteSize;
			spriteComponent.height = spriteSize;
			spriteComponent.texture = m_animTextures[m_currAnimTextureIndex];
			m_registry.emplace<SpriteComponent>(m_animSprite, spriteComponent);
		}
	}

	void Demo08_Scene::updateSprites(float dt)
	{
		// Get mouse position in world space
		const glm::vec2 mousePos = Renderer2DSubsystem::getMousePosition_ECS(m_registry);
		// Get mouse strength parameter from GUI
		const float mouseStrength = m_guiWindow->getMouseStrength();

		// Update each sprite's position, rotation and scale
		for (size_t i = 0; i < m_spritesCount; i++)
		{
			// Get sprite's transform component
			TransformComponent2D& transform = m_registry.get<TransformComponent2D>(m_sprites[i]);

			// Calculate vector from sprite to mouse
			const glm::vec2 vecSpriteToMouse = transform.position - mousePos;
			// Calculate distance from sprite to mouse
			const float distSpriteToMouse = glm::length(vecSpriteToMouse);

			// Apply "Anti-Gravity Mouse Field" effect,
			// increasing sprite's velocity with some amount inversely proportional to its distance to the mouse
			m_spritesVelocities[i] += (vecSpriteToMouse / distSpriteToMouse) * mouseStrength / (0.3f + 2.0f / mouseStrength * std::abs(distSpriteToMouse));

			// Move sprite with its velocity
			transform.position += m_spritesVelocities[i];
			transform.move(m_spritesVelocities[i]);
			// Move sprite randomly
			transform.move(getRandomVec2
			(
				{ -float((i * 4 + 2) % 30) * 0.8f, -float((i * 7 + 6) % 30) * 0.8f },
				{ float((i * 3 + 11) % 30) * 0.8f, float((i * 11 + 3) % 30) * 0.8f }
			) * dt);

			// Apply some "air friction" decreasing sprite's velocity with time
			m_spritesVelocities[i] *= 0.95f;

			// Rotate sprite randomly
			transform.rotation = dt * sin(t * float(i % 7)) * float(i % 17) / 1.5f;
			// Scale sprite randomly
			transform.scaleFactor *= getRandomVec2
			(
				{ 0.98f, 0.98f },
				{ 1.02f, 1.02f }
			);
		}
	}

	void Demo08_Scene::updateAnimSprite(float dt)
	{
		m_timeTilAnimUpdate -= dt;
		if (m_timeTilAnimUpdate <= 0.0f)
		{
			// Change sprite's texture to be the next texture of the animation
			m_currAnimTextureIndex = (m_currAnimTextureIndex + 1) % m_animTextures.size();
			m_registry.get<SpriteComponent>(m_animSprite).texture = m_animTextures[m_currAnimTextureIndex];
			// Set time until next animation update according to animation speed parameter in GUI
			m_timeTilAnimUpdate = 1.0f / m_guiWindow->getAnimSpeed();
		}
	}

} // namespace Demo