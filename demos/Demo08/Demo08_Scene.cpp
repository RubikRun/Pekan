#include "Demo08_Scene.h"
#include "PekanLogger.h"
#include "Utils/RandomizationUtils.h"
#include "PekanTools.h"
#include "RenderCommands.h"
#include "Renderer2DSubsystem.h"
#include "Image.h"

#include <algorithm>

constexpr float ZOOM_SPEED = 1.1f;

using namespace Pekan;
using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;
using namespace Pekan::RandomizationUtils;
using namespace Pekan::Tools;

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

    bool Demo08_Scene::init()
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

		m_spritesCount = m_guiWindow->getNumberOfSprites();
		m_spritesMaxCount = m_guiWindow->getMaxNumberOfSprites();

		createCamera();
		createSprites();
		createCenterSquare();
		createAnimSprite();

        return true;
	}

	void Demo08_Scene::update(double dt)
	{
		PK_ASSERT_QUICK(m_guiWindow != nullptr);
		m_spritesCount = m_guiWindow->getNumberOfSprites();

		updateSprites(float(dt));
		updateAnimSprite(float(dt));

		t += float(dt);
	}

	void Demo08_Scene::render() const
	{
		Renderer2DSubsystem::beginFrame();
		RenderCommands::clear();

		for (size_t i = 0; i < m_spritesCount; i++)
		{
			m_sprites[i].render();
		}
		m_centerSquare.render();
		m_animSprite.render();

		Renderer2DSubsystem::endFrame();
	}

	void Demo08_Scene::exit()
	{
		for (int i = 0; i < m_sprites.size(); i++)
		{
			m_sprites[i].destroy();
		}
		m_centerSquare.destroy();
		for (int i = 0; i < m_animTextures.size(); i++)
		{
			m_animTextures[i]->destroy();
		}
		m_animSprite.destroy();
		m_camera->destroy();
	}

	void Demo08_Scene::createCamera()
	{
		const glm::vec2 windowSize = glm::vec2(PekanEngine::getWindow().getSize());

		m_camera = std::make_shared<Camera2D>();
		m_camera->create(windowSize.x, windowSize.y);

		Renderer2DSubsystem::setCamera(m_camera);
		PekanTools::enableCameraController2D(m_camera);
		PekanTools::setCameraController2DZoomSpeed(1.1f);
	}

	void Demo08_Scene::createSprites()
	{
		const glm::vec2 windowSize = glm::vec2(PekanEngine::getWindow().getSize());

		// Load textures
		std::vector<Texture2D_Ptr> textures;
		loadSpritesTextures(textures);

		// Define randomization parameters
		const float minDim = std::min(windowSize.x, windowSize.y);
		const glm::vec2 widthHeightRange =
		{
			minDim * 0.01f,
			minDim * 0.04f
		};
		const glm::vec2 positionMin = { -windowSize.x / 2.0f + widthHeightRange.y / 2.0f, -windowSize.y / 2.0f + widthHeightRange.y / 2.0f };
		const glm::vec2 positionMax = { windowSize.x / 2.0f - widthHeightRange.y / 2.0f, windowSize.y / 2.0f - widthHeightRange.y / 2.0f };

		// Generate sprites with random textures with a random size at a random position
		m_sprites.resize(m_spritesMaxCount);
		for (size_t i = 0; i < m_spritesMaxCount; i++)
		{
			const size_t textureIndex = getRandomInt(0, textures.size() - 1);
			m_sprites[i].create
			(
				textures[textureIndex],
				getRandomFloat(widthHeightRange.x, widthHeightRange.y),
				getRandomFloat(widthHeightRange.x, widthHeightRange.y)
			);
			m_sprites[i].setPosition(getRandomVec2(positionMin, positionMax));
		}

		// Initialize velocities list with { 0, 0 } velocity for each sprite
		m_spritesVelocities = std::vector<glm::vec2>(m_spritesMaxCount, { 0.0f, 0.0f });
	}

	void Demo08_Scene::createCenterSquare()
	{
		m_centerSquare.create(100.0f, 100.0f);
		m_centerSquare.setColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	}

	void Demo08_Scene::createAnimSprite()
	{
		const glm::vec2 windowSize = glm::vec2(PekanEngine::getWindow().getSize());

		// Load animation textures
		loadAnimTextures(m_animTextures);

		const float spriteSize = windowSize.x * ANIMATION_SIZE_FACTOR;
		m_currAnimTextureIndex = 0;
		m_animSprite.create(m_animTextures[m_currAnimTextureIndex], spriteSize, spriteSize);
		m_animSprite.setPosition({ -windowSize.x / 2.0f + spriteSize / 2.0f, windowSize.y / 2.0f - spriteSize / 2.0f });
	}

	void Demo08_Scene::updateSprites(float dt)
	{
		// Get mouse position in world space
		const glm::vec2 mousePos = Renderer2DSubsystem::getMousePosition();
		// Get mouse strength parameter from GUI
		const float mouseStrength = m_guiWindow->getMouseStrength();

		// Update each sprite's position, rotation and scale
		for (size_t i = 0; i < m_spritesCount; i++)
		{
			// Calculate vector from sprite to mouse
			const glm::vec2 vecSpriteToMouse = m_sprites[i].getPosition() - mousePos;
			// Calculate distance from sprite to mouse
			const float distSpriteToMouse = glm::length(vecSpriteToMouse);

			// Apply "Anti-Gravity Mouse Field" effect,
			// increasing sprite's velocity with some amount inversely proportional to its distance to the mouse
			m_spritesVelocities[i] += (vecSpriteToMouse / distSpriteToMouse) * mouseStrength / (0.3f + 2.0f / mouseStrength * std::abs(distSpriteToMouse));

			// Move sprite with its velocity
			m_sprites[i].move(m_spritesVelocities[i]);
			// Move sprite randomly
			m_sprites[i].move(getRandomVec2
			(
				{ -float((i * 4 + 2) % 30) * 0.8f, -float((i * 7 + 6) % 30) * 0.8f },
				{ float((i * 3 + 11) % 30) * 0.8f, float((i * 11 + 3) % 30) * 0.8f }
			) * dt);

			// Apply some "air friction" decreasing sprite's velocity with time
			m_spritesVelocities[i] *= 0.95f;

			// Rotate sprite randomly
			m_sprites[i].setRotation(dt * sin(t * float(i % 7)) * float(i % 17) / 1.5f);
			// Scale sprite randomly
			m_sprites[i].setScale(m_sprites[i].getScale() * getRandomVec2
			(
				{ 0.98f, 0.98f },
				{ 1.02f, 1.02f }
			));
		}
	}

	void Demo08_Scene::updateAnimSprite(float dt)
	{
		m_timeTilAnimUpdate -= dt;
		if (m_timeTilAnimUpdate <= 0.0f)
		{
			// Change sprite's texture to be the next texture of the animation
			m_currAnimTextureIndex = (m_currAnimTextureIndex + 1) % m_animTextures.size();
			m_animSprite.setTexture(m_animTextures[m_currAnimTextureIndex]);
			// Set time until next animation update according to animation speed parameter in GUI
			m_timeTilAnimUpdate = 1.0f / m_guiWindow->getAnimSpeed();
		}
	}

} // namespace Demo