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
		PekanRenderer::enableMultisampleAntiAliasing();

		createCamera();
		createCoordSys();

		m_rectangle.create(1.0f, 1.5f);
		m_rectangle.setPosition({ 1.5f, 1.0f });
		m_rectangle.setColor({ 1.0f, 0.0f, 0.0f, 1.0f });

		m_circle.create(0.6f);
		m_circle.setPosition({ -1.5f, 1.0f });
		m_circle.setColor({ 0.0f, 1.0f, 0.0f, 1.0f });

		m_circleStatic.create(0.6f);
		m_circleStatic.setPosition({ 0.0f, 0.0f });
		m_circleStatic.setColor({ 1.0f, 0.0f, 1.0f, 1.0f });

		m_triangle.create(glm::vec2(-0.5, -0.5f),glm::vec2(0.5f, -0.5f), glm::vec2(0.0f, 0.5f));
		m_triangle.setPosition({ 1.5f, -1.0f });
		m_triangle.setColor({ 0.0f, 0.0f, 1.0f, 1.0f });

		m_polygon.create
		({
			{  0.3f,  0.6f },
			{  0.1f,  0.9f },
			{ -0.3f,  0.8f },
			{ -0.5f,  0.3f },
			{ -0.1f, -0.2f },
			{  0.5f,  0.0f }
		});
		m_polygon.setPosition({ -1.5f, -2.0f });
		m_polygon.setColor({ 1.0f, 1.0f, 0.0f, 1.0f });

		m_line.create({ -2.0f, -1.2f }, { 1.2f, 2.0f }, 0.02f);
		m_line.setPosition({ 0.5f, 0.5f });
		m_line.setColor({ 0.0f, 1.0f, 1.0f, 1.0f });

		return true;
	}

	void Demo07_Scene::update(double dt)
	{
		m_rectangle.setPosition(m_guiWindow->getRectanglePosition());
		m_rectangle.setRotation(m_guiWindow->getRectangleRotation());
		m_rectangle.setScale(m_guiWindow->getRectangleScale());

		m_circle.setPosition(m_guiWindow->getCirclePosition());
		m_circle.setRotation(m_guiWindow->getCircleRotation());
		m_circle.setScale(m_guiWindow->getCircleScale());

		m_circleStatic.setPosition(m_guiWindow->getCircleStaticPosition());
		m_circleStatic.setRotation(m_guiWindow->getCircleStaticRotation());
		m_circleStatic.setScale(m_guiWindow->getCircleStaticScale());

		m_triangle.setPosition(m_guiWindow->getTrianglePosition());
		m_triangle.setRotation(m_guiWindow->getTriangleRotation());
		m_triangle.setScale(m_guiWindow->getTriangleScale());

		m_polygon.setPosition(m_guiWindow->getPolygonPosition());
		m_polygon.setRotation(m_guiWindow->getPolygonRotation());
		m_polygon.setScale(m_guiWindow->getPolygonScale());

		m_line.setPosition(m_guiWindow->getLinePosition());
		m_line.setRotation(m_guiWindow->getLineRotation());
		m_line.setScale(m_guiWindow->getLineScale());
	}

	void Demo07_Scene::render()
	{
		PekanRenderer::clear();
		m_rectangle.render(*m_camera);
		m_circle.render(*m_camera);
		m_circleStatic.render(*m_camera);
		m_triangle.render(*m_camera);
		m_polygon.render(*m_camera);
		m_line.render(*m_camera);
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
		m_rectangle.destroy();
		m_circle.destroy();
		m_circleStatic.destroy();
		m_triangle.destroy();
		m_polygon.destroy();
		m_line.destroy();
	}

	void Demo07_Scene::createCamera()
	{
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
	}

	void Demo07_Scene::createCoordSys()
	{
		// Number of divisions rendered on the coordinate system
		// in each of the 4 directions, starting from the center
		const int divsCount = int(CAMERA_SIZE) * 10;

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
	}

} // namespace Demo