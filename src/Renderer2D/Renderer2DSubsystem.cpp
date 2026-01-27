#include "Renderer2DSubsystem.h"

#include "PekanLogger.h"
#include "SubsystemManager.h"
#include "GraphicsSubsystem.h"
#include "ShaderPreprocessor.h"
#include "CameraSystem2D.h"

using namespace Pekan::Graphics;

namespace Pekan
{
namespace Renderer2D
{

	static Renderer2DSubsystem g_renderer2DSubsystem;
	
	void Renderer2DSubsystem::registerAsSubsystem()
	{
		SubsystemManager::registerSubsystem(&g_renderer2DSubsystem);
	}

	void Renderer2DSubsystem::beginFrame()
	{
	}

	void Renderer2DSubsystem::endFrame()
	{
	}

	glm::vec2 Renderer2DSubsystem::getMousePosition_ECS(const entt::registry& registry)
	{
		const CameraComponent2D& camera = CameraSystem2D::getPrimaryCamera(registry);

		// Get mouse position in window space
		const glm::vec2 mousePositionInWindow = PekanEngine::getMousePosition();
		// Convert mouse position from window space to world space
		const glm::vec2 mousePositionInWorld = camera.windowToWorldPosition(mousePositionInWindow);
		return mousePositionInWorld;
	}

	bool Renderer2DSubsystem::init()
	{
		return true;
	}

	void Renderer2DSubsystem::exit()
	{
	}

	ISubsystem* Renderer2DSubsystem::getParent()
	{
		return GraphicsSubsystem::getInstance();
	}

} // namespace Renderer2D
} // namespace Pekan