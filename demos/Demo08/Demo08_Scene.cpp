#include "Demo08_Scene.h"
#include "PekanLogger.h"
#include "Utils/PekanUtils.h"
#include "PekanTools.h"
#include "RenderCommands.h"
#include "Renderer2DSystem.h"
#include "Image.h"

#include <algorithm>

static const float ZOOM_SPEED = 1.1f;

using namespace Pekan;
using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;
using namespace Pekan::Utils;
using namespace Pekan::Tools;

namespace Demo
{

	// Number of textures to be used for the sprites.
	// Textures will be loaded from .png files that are expected to be under "resources" directory
	// and named "00.png", "01.png", "02.png", etc.
	static constexpr int TEXTURES_COUNT = 44;

	// Oscillates between 0 and 1 in a sine wave, as x grows
	static float osc(float x)
	{
		return (cos(x) + 1.0f) / 2.0f;
	}
	// Oscillates between a and b in a sine wave, as x grows
	static float osc(float x, float a, float b)
	{
		return a + (b - a) * osc(x);
	}

	// Loads textures to be used for the sprites
	//
	// NOTE: this works only for two-digit filenames, so it supports at most 100 textures (00 to 99).
	static void loadTextures(std::vector<Texture2D_Ptr>& textures)
	{
		textures.clear();
		textures.resize(TEXTURES_COUNT);
		for (size_t i = 0; i < TEXTURES_COUNT; i++)
		{
			// Generate image file's name
			std::string filename = "resources/";
			if (i < 10) filename += "0";
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

        return true;
	}

	void Demo08_Scene::update(double dt)
	{
		if (m_guiWindow != nullptr)
		{
			m_spritesCount = m_guiWindow->getNumberOfSprites();
		}

		updateSprites(float(dt));

		t += float(dt);
	}

	void Demo08_Scene::render() const
	{
		Renderer2DSystem::beginFrame();
		RenderCommands::clear();

		m_centerSquare.render();
		for (size_t i = 0; i < m_spritesCount; i++)
		{
			m_sprites[i].render();
		}

		Renderer2DSystem::endFrame();
	}

	void Demo08_Scene::exit()
	{
		for (size_t i = 0; i < m_sprites.size(); i++)
		{
			m_sprites[i].destroy();
		}
		m_centerSquare.destroy();
	}

	void Demo08_Scene::createCamera()
	{
		const glm::vec2 windowSize = glm::vec2(PekanEngine::getWindow().getSize());

		m_camera = std::make_shared<Camera2D>();
		m_camera->setSize(windowSize.x, windowSize.y);

		Renderer2DSystem::setCamera(m_camera);
		PekanTools::enableCameraController2D(m_camera);
		PekanTools::setCameraController2DZoomSpeed(1.1f);
	}

	void Demo08_Scene::createSprites()
	{
		const glm::vec2 windowSize = glm::vec2(PekanEngine::getWindow().getSize());

		// Load textures
		std::vector<Texture2D_Ptr> textures;
		loadTextures(textures);

		// Define randomization parameters
		const float minDim = std::min(windowSize.x, windowSize.y);
		const glm::vec2 widthHeightRange =
		{
			minDim * 0.01f,
			minDim * 0.04f
		};
		const glm::vec2 positionXRange = { -windowSize.x / 2.0f + widthHeightRange.y / 2.0f, windowSize.x / 2.0f - widthHeightRange.y / 2.0f };
		const glm::vec2 positionYRange = { -windowSize.y / 2.0f + widthHeightRange.y / 2.0f, windowSize.y / 2.0f - widthHeightRange.y / 2.0f };

		// Generate sprites with random textures with a random size at a random position
		m_sprites.resize(m_spritesMaxCount);
		for (size_t i = 0; i < m_spritesMaxCount; i++)
		{
			const size_t textureIndex = getRandomInt(0, textures.size() - 1);
			m_sprites[i].create(textures[textureIndex], getRandomFloat(widthHeightRange), getRandomFloat(widthHeightRange));
			m_sprites[i].setPosition(getRandomVec2(positionXRange, positionYRange));
		}

		// Initialize velocities list with { 0, 0 } velocity for each sprite
		m_spritesVelocities = std::vector<glm::vec2>(m_spritesMaxCount, { 0.0f, 0.0f });
	}

	void Demo08_Scene::createCenterSquare()
	{
		m_centerSquare.create(100.0f, 100.0f, false);
		m_centerSquare.setColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	}

	void Demo08_Scene::updateSprites(float dt)
	{
		// Get mouse position in world space
		const glm::vec2 mousePos = Renderer2DSystem::getMousePosition();
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
			// Move sprite randomly a little bit
			m_sprites[i].move(getRandomVec2
			(
				{ -float((i * 4 + 2) % 30) * 0.8f, float((i * 3 + 11) % 30) * 0.8f },
				{ -float((i * 7 + 6) % 30) * 0.8f, float((i * 11 + 3) % 30) * 0.8f }
			) * dt);

			// Apply some "air friction" decreasing sprite's velocity with time
			m_spritesVelocities[i] *= 0.95f;

			// Rotate sprite randomly
			m_sprites[i].setRotation(dt * sin(t * float(i % 7)) * float(i % 17) / 1.5f);
			// Scale sprite randomly
			m_sprites[i].setScale(m_sprites[i].getScale() * getRandomVec2
			(
				{ 0.98f, 1.02f },
				{ 0.98f, 1.02f }
			));
		}
	}

} // namespace Demo