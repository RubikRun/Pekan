#include "Renderer2DSystem.h"

#include "PekanLogger.h"
#include "SubsystemManager.h"
#include "GraphicsSystem.h"
#include "ShaderPreprocessor.h"

using namespace Pekan::Graphics;

namespace Pekan
{
namespace Renderer2D
{

	// A list of .pkshad files that need to be preprocessed when Renderer2D is initialized
	static const size_t PKSHAD_FILES_COUNT = 2;
	static const char* PKSHAD_FILES[PKSHAD_FILES_COUNT] =
	{
		PEKAN_RENDERER2D_ROOT_DIR "/Shaders/2D_Batch_1DTexture_FragmentShader.pkshad",
		PEKAN_RENDERER2D_ROOT_DIR "/Shaders/2D_Batch_FragmentShader.pkshad"
	};

	// Preprocesses all .pkshad files needed by Renderer2D
	static void preprocessPkshadFiles();

	static Renderer2DSystem g_renderer2DSystem;
	
	void Renderer2DSystem::registerSubsystem()
	{
		SubsystemManager::registerSubsystem(&g_renderer2DSystem);
	}

	Camera2D_ConstWeakPtr Renderer2DSystem::s_camera;
	RenderBatch2D Renderer2DSystem::s_batch;

	void Renderer2DSystem::beginFrame()
	{
		s_batch.clear();
	}

	void Renderer2DSystem::endFrame()
	{
		Camera2D_ConstPtr camera = s_camera.lock();
		s_batch.render(camera);
	}

	glm::vec2 Renderer2DSystem::getMousePosition()
	{
		Camera2D_ConstPtr camera = s_camera.lock();

		if (camera == nullptr)
		{
			PK_LOG_ERROR("Trying to get mouse position in camera space, but there is no camera set in Renderer2DSystem.", "Pekan");
			return { -1.0f, -1.0f };
		}

		// Get mouse position in window space
		const glm::vec2 mousePosWindow = PekanEngine::getMousePosition();
		// Convert mouse position from window space to world space
		const glm::vec2 mousePosWorld = camera->windowToWorld(mousePosWindow);
		return mousePosWorld;
	}

	bool Renderer2DSystem::init()
	{
		preprocessPkshadFiles();
		s_batch.create();

		return true;
	}

	void Renderer2DSystem::exit()
	{
		s_batch.destroy();
	}

	ISubsystem* Renderer2DSystem::getParent()
	{
		return GraphicsSystem::getInstance();
	}

	void Renderer2DSystem::submitForRendering(const Shape& shape)
	{
		// Add shape to batch.
		// If it couldn't be added, this means that the batch is full,
		if (!s_batch.addShape(shape))
		{
			Camera2D_ConstPtr camera = s_camera.lock();
			// so we can render the batch and clear it, effectively starting a new one.
			s_batch.render(camera);
			s_batch.clear();
			// Finally we need to add the shape to the new batch.
			// If it couldn't be added again, to a fresh new batch, something is definitely wrong.
			if (!s_batch.addShape(shape))
			{
				PK_LOG_ERROR("Failed to add a shape to the internal RenderBatch2D that was just cleared.", "Pekan");
			}
		}
	}

	void Renderer2DSystem::submitForRendering(const Sprite& sprite)
	{
		// Add sprite to batch.
		// If it couldn't be added, this means that the batch is full,
		if (!s_batch.addSprite(sprite))
		{
			Camera2D_ConstPtr camera = s_camera.lock();
			// so we can render the batch and clear it, effectively starting a new one.
			s_batch.render(camera);
			s_batch.clear();
			// Finally we need to add the sprite to the new batch.
			// If it couldn't be added again, to a fresh new batch, something is definitely wrong.
			if (!s_batch.addSprite(sprite))
			{
				PK_LOG_ERROR("Failed to add a sprite to the internal RenderBatch2D that was just cleared.", "Pekan");
			}
		}
	}

	static void preprocessPkshadFiles()
	{
		const int maxTextureSlots = RenderState::getMaxTextureSlots();
		const std::string maxTextureSlotsString = std::to_string(maxTextureSlots);

		// A list of substitution lists, one for each .pkshad file
		const std::unordered_map<std::string, std::string> PKSHAD_FILES_SUBSTITUTIONS[PKSHAD_FILES_COUNT] =
		{
			{
				{ "MAX_TEXTURE_SLOTS", maxTextureSlotsString }
			},
			{
				{ "MAX_TEXTURE_SLOTS", maxTextureSlotsString }
			}
		};

		// Preprocess each .pkshad file
		for (size_t i = 0; i < PKSHAD_FILES_COUNT; i++)
		{
			ShaderPreprocessor::preprocess
			(
				PKSHAD_FILES[i],
				PKSHAD_FILES_SUBSTITUTIONS[i]
			);
		}
	}

} // namespace Renderer2D
} // namespace Pekan