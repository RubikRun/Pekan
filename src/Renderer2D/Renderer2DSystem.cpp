#include "Renderer2DSystem.h"

#include "PekanLogger.h"
#include "SubsystemManager.h"
#include "GraphicsSystem.h"

using namespace Pekan::Graphics;

namespace Pekan
{
namespace Renderer2D
{

	static Renderer2DSystem g_renderer2DSystem;
	
	void Renderer2DSystem::registerSubsystem()
	{
		SubsystemManager::registerSubsystem(&g_renderer2DSystem);
	}

	Camera2D_ConstWeakPtr Renderer2DSystem::s_camera;
	ShapesBatch Renderer2DSystem::s_shapesBatchDynamic;
	ShapesBatch Renderer2DSystem::s_shapesBatchStatic;
	SpritesBatch Renderer2DSystem::s_spritesBatchDynamic;
	SpritesBatch Renderer2DSystem::s_spritesBatchStatic;

	void Renderer2DSystem::beginFrame()
	{
		s_shapesBatchDynamic.clear();
		s_shapesBatchStatic.clear();
		s_spritesBatchDynamic.clear();
		s_spritesBatchStatic.clear();
	}

	void Renderer2DSystem::endFrame()
	{
		renderShapesBatch(s_shapesBatchDynamic);
		renderShapesBatch(s_shapesBatchStatic);
		renderSpritesBatch(s_spritesBatchDynamic);
		renderSpritesBatch(s_spritesBatchStatic);
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
		s_shapesBatchDynamic.create(BufferDataUsage::DynamicDraw);
		s_shapesBatchStatic.create(BufferDataUsage::StaticDraw);
		s_spritesBatchDynamic.create(BufferDataUsage::DynamicDraw);
		s_spritesBatchStatic.create(BufferDataUsage::StaticDraw);

		return true;
	}

	void Renderer2DSystem::exit()
	{
		s_shapesBatchDynamic.destroy();
		s_shapesBatchStatic.destroy();
		s_spritesBatchDynamic.destroy();
		s_spritesBatchStatic.destroy();
	}

	ISubsystem* Renderer2DSystem::getParent()
	{
		return GraphicsSystem::getInstance();
	}

	void Renderer2DSystem::render(const Shape& shape)
	{
		if (shape.isDynamic())
		{
			// Add shape to dynamic batch.
			// If batch is full, render it, and clear it, effectively starting a new one.
			if (!s_shapesBatchDynamic.addShape(shape))
			{
				renderShapesBatch(s_shapesBatchDynamic);
				s_shapesBatchDynamic.clear();
			}
		}
		else
		{
			// Add shape to static batch.
			// If batch is full, render it, and clear it, effectively starting a new one.
			if (!s_shapesBatchStatic.addShape(shape))
			{
				renderShapesBatch(s_shapesBatchStatic);
				s_shapesBatchStatic.clear();
			}
		}
	}

	void Renderer2DSystem::renderShapesBatch(ShapesBatch& batch)
	{
		Camera2D_ConstPtr camera = s_camera.lock();

		if (camera != nullptr)
		{
			batch.render(camera);
		}
		else
		{
			batch.render();
		}
	}

	void Renderer2DSystem::render(const Sprite& sprite)
	{
		if (sprite.isDynamic())
		{
			// Add sprite to dynamic batch.
			// If batch is full, render it, and clear it, effectively starting a new one.
			if (!s_spritesBatchDynamic.addSprite(sprite))
			{
				renderSpritesBatch(s_spritesBatchDynamic);
				s_spritesBatchDynamic.clear();
			}
		}
		else
		{
			// Add sprite to static batch.
			// If batch is full, render it, and clear it, effectively starting a new one.
			if (!s_spritesBatchStatic.addSprite(sprite))
			{
				renderSpritesBatch(s_spritesBatchStatic);
				s_spritesBatchStatic.clear();
			}
		}
	}

	void Renderer2DSystem::renderSpritesBatch(SpritesBatch& batch)
	{
		Camera2D_ConstPtr camera = s_camera.lock();

		if (camera != nullptr)
		{
			batch.render(camera);
		}
		else
		{
			batch.render();
		}
	}

} // namespace Renderer2D
} // namespace Pekan