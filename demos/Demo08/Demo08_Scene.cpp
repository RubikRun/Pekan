#include "Demo08_Scene.h"
#include "PekanLogger.h"
#include "Utils/PekanUtils.h"
#include "PekanTools.h"
#include "RenderCommands.h"
#include "Renderer2DSystem.h"

#include <algorithm>

static const float ZOOM_SPEED = 1.1f;

using namespace Pekan;
using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;
using namespace Pekan::Utils;
using namespace Pekan::Tools;

namespace Demo
{

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

    bool Demo08_Scene::init()
	{
		if (m_guiWindow == nullptr)
		{
			PK_LOG_ERROR("Cannot initialize Demo08_Scene because there is no GUI window attached.", "Demo08");
			return false;
		}

		m_spritesCount = m_guiWindow->getNumberOfSprites();
		m_spritesMaxCount = m_guiWindow->getMaxNumberOfSprites();

		createCamera();
		createCenterSquare();

        return true;
	}

	void Demo08_Scene::update(double dt)
	{
		if (m_guiWindow != nullptr)
		{
			m_spritesCount = m_guiWindow->getNumberOfSprites();
		}

		t += float(dt);
	}

	void Demo08_Scene::render() const
	{
		Renderer2DSystem::beginFrame();
		RenderCommands::clear();

		m_centerSquare.render();

		Renderer2DSystem::endFrame();
	}

	void Demo08_Scene::exit()
	{
		m_centerSquare.destroy();
	}

	void Demo08_Scene::createCamera()
	{
		const glm::vec2 windowSize = PekanEngine::getWindow().getSize();

		m_camera = std::make_shared<Camera2D>();
		m_camera->setSize(windowSize.x, windowSize.y);

		Renderer2DSystem::setCamera(m_camera);
		PekanTools::enableCameraController2D(m_camera);
		PekanTools::setCameraController2DZoomSpeed(1.1f);
	}

	void Demo08_Scene::createCenterSquare()
	{
		m_centerSquare.create(100.0f, 100.0f, false);
		m_centerSquare.setColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	}

} // namespace Demo