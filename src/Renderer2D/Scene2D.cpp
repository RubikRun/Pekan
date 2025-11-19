#include "Scene2D.h"

#include "Renderer2DSystem_ECS.h"
#include "RenderCommands.h"
#include "RenderState.h"
#include "PostProcessor.h"

using namespace Pekan::Graphics;

namespace Pekan
{
namespace Renderer2D {

	bool Scene2D::init()
	{
		// Create and initialize scene's camera controller
		m_cameraController = std::make_shared<CameraController2D>();
		m_cameraController->init(this);

		return _init();
	}

	void Scene2D::exit()
	{
		_exit();
	}

	void Scene2D::render() const
	{
		PostProcessor::beginFrame();

		const entt::registry& registry = getRegistry();
		Renderer2DSystem_ECS::render(registry);

		_render();

		PostProcessor::endFrame();
	}

} // namespace Renderer2D
} // namespace Pekan