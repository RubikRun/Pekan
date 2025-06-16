#include "Demo07_Scene.h"

#include "Logger/PekanLogger.h"
#include "Utils/PekanUtils.h"
#include "PekanRenderer.h"
#include "PekanEngine.h"
#include "PekanTools.h"

using namespace Pekan;
using namespace Pekan::Renderer;
using namespace Pekan::Tools;

static const float CAMERA_SIZE = 10.0f;
static const float DIVS_LENGTH = 0.1f;
static const glm::vec4 COORD_SYS_COLOR = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
static const float COORD_SYS_THICKNESS = 0.02f;

namespace Demo
{

    bool Demo07_Scene::init()
	{
		/////////
		// CAMERA
		/////////

		m_camera = std::make_shared<Camera2D>();
		// TODO: cleanup this logic into an overload of Camera2D::setSize()
		const glm::ivec2 winRes = PekanEngine::getWindowResolution();
		if (winRes.x > winRes.y)
		{
			m_camera->setSize(CAMERA_SIZE * float(winRes.x) / float(winRes.y), CAMERA_SIZE);
		}
		else
		{
			m_camera->setSize(CAMERA_SIZE, CAMERA_SIZE * float(winRes.y) / float(winRes.x));
		}
		PekanTools::enableCameraController2D(m_camera);

		////////////////////
		// COORDINATE SYSTEM
		////////////////////

		// Number of divisions rendered on the coordinate system
		// in each of the 4 directions, starting from the center
		const int divsCount = int(CAMERA_SIZE);

		m_coordSys.resize(divsCount * 4 + 2);
		// Create horizontal and vertical line
		m_coordSys[0].create({ -9999.0f, 0.0f }, { 9999.0f, 0.0f });
		m_coordSys[1].create({ 0.0f, -9999.0f }, { 0.0f, 9999.0f });
		// Create division lines in 4 directions
		for (int i = 0; i < divsCount; i++)
		{
			m_coordSys[2 + i].create({ float(i + 1), -DIVS_LENGTH }, { float(i + 1), DIVS_LENGTH });
		}
		for (int i = 0; i < divsCount; i++)
		{
			m_coordSys[2 + divsCount + i].create({ float(-1 - i), -DIVS_LENGTH }, { float(-1 - i), DIVS_LENGTH });
		}
		for (int i = 0; i < divsCount; i++)
		{
			m_coordSys[2 + divsCount * 2 + i].create({ -DIVS_LENGTH, float(i + 1) }, { DIVS_LENGTH, float(i + 1) });
		}
		for (int i = 0; i < divsCount; i++)
		{
			m_coordSys[2 + divsCount * 3 + i].create({ -DIVS_LENGTH, float(-1 - i) }, { DIVS_LENGTH, float(-1 - i) });
		}
		// Set color and thickness of coordinate system
		for (LineShape& csLine : m_coordSys)
		{
			csLine.setColor(COORD_SYS_COLOR);
			csLine.setThickness(COORD_SYS_THICKNESS);
		}

        return true;
	}

	void Demo07_Scene::update(double dt)
	{
	}

	void Demo07_Scene::render()
	{
		PekanRenderer::clear();
		for (LineShape& csLine : m_coordSys)
		{
			csLine.render(*m_camera);
		}
	}

	void Demo07_Scene::exit()
	{
		for (LineShape& csLine : m_coordSys)
		{
			csLine.destroy();
		}
	}

} // namespace Demo