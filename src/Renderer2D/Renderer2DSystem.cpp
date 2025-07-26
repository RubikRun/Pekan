#include "Renderer2DSystem.h"

#include "SubsystemManager.h"
#include "Graphics.h"

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
	ShapesBatch Renderer2DSystem::s_batchDynamic;
	ShapesBatch Renderer2DSystem::s_batchStatic;

	void Renderer2DSystem::beginFrame()
	{
		s_batchDynamic.clear();
		s_batchStatic.clear();
	}

	void Renderer2DSystem::endFrame()
	{
		renderBatch(s_batchDynamic);
		renderBatch(s_batchStatic);
	}

	bool Renderer2DSystem::init()
	{
		s_batchDynamic.create(BufferDataUsage::DynamicDraw);
		s_batchStatic.create(BufferDataUsage::StaticDraw);

		return true;
	}

	void Renderer2DSystem::exit()
	{
		s_batchDynamic.destroy();
		s_batchStatic.destroy();
	}

	ISubsystem* Renderer2DSystem::getParent()
	{
		return Graphics::Graphics::getInstance();
	}

	void Renderer2DSystem::render(const Shape& shape)
	{
		if (shape.isDynamic())
		{
			// Add shape to dynamic batch.
			// If batch is full, render it, and clear it, effectively starting a new one.
			if (!s_batchDynamic.addShape(shape))
			{
				renderBatch(s_batchDynamic);
				s_batchDynamic.clear();
			}
		}
		else
		{
			// Add shape to static batch.
			// If batch is full, render it, and clear it, effectively starting a new one.
			if (!s_batchStatic.addShape(shape))
			{
				renderBatch(s_batchStatic);
				s_batchStatic.clear();
			}
		}
	}

	void Renderer2DSystem::renderBatch(ShapesBatch& batch)
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