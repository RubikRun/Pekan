#include "Renderer2D.h"

#include "SubsystemManager.h"
#include "Graphics.h"

using namespace Pekan::Graphics;

namespace Pekan
{
namespace Renderer
{

	static Renderer2D g_renderer2D;
	
	void Renderer2D::registerSubsystem()
	{
		SubsystemManager::registerSubsystem(&g_renderer2D);
	}

	Camera2D_ConstWeakPtr Renderer2D::s_camera;
	ShapesBatch Renderer2D::s_batchDynamic;
	ShapesBatch Renderer2D::s_batchStatic;

	void Renderer2D::beginFrame()
	{
		s_batchDynamic.clear();
		s_batchStatic.clear();
	}

	void Renderer2D::endFrame()
	{
		renderBatch(s_batchDynamic);
		renderBatch(s_batchStatic);
	}

	void Renderer2D::init()
	{
		s_batchDynamic.create(BufferDataUsage::DynamicDraw);
		s_batchStatic.create(BufferDataUsage::StaticDraw);

		m_isInitialized = true;
	}

	void Renderer2D::exit()
	{
		s_batchDynamic.destroy();
		s_batchStatic.destroy();

		m_isInitialized = false;
	}

	ISubsystem* Renderer2D::getParent()
	{
		return Graphics::Graphics::getInstance();
	}

	void Renderer2D::render(const Shape& shape)
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

	void Renderer2D::renderBatch(ShapesBatch& batch)
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

} // namespace Renderer
} // namespace Pekan