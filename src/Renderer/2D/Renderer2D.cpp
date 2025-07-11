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

	Camera2DWeakPtr Renderer2D::s_camera;
	ShapesBatch Renderer2D::s_batchDynamic;
	ShapesBatch Renderer2D::s_batchStatic;

	void Renderer2D::beginFrame()
	{
		s_batchDynamic.clear();
		s_batchStatic.clear();
	}

	void Renderer2D::endFrame()
	{
		Camera2DPtr camera = s_camera.lock();

		if (camera != nullptr)
		{
			s_batchDynamic.render(camera);
			s_batchStatic.render(camera);
		}
		else
		{
			s_batchDynamic.render();
			s_batchStatic.render();
		}
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
			s_batchDynamic.addShape(shape);
		}
		else
		{
			s_batchStatic.addShape(shape);
		}
	}

} // namespace Renderer
} // namespace Pekan