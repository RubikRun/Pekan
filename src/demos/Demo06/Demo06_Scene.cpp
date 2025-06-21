#include "Demo06_Scene.h"
#include "Logger/PekanLogger.h"
#include "Utils/PekanUtils.h"
#include "PekanTools.h"

#include "Events/MouseEvents.h"
#include "Events/KeyEvents.h"

using namespace Pekan;
using namespace Pekan::Renderer;
using namespace Pekan::Utils;
using namespace Pekan::Tools;

static const char* vertexShaderFilePath = "resources/06_vertex_shader.glsl";
static const char* fragmentShaderFilePath = "resources/06_fragment_shader.glsl";

static glm::vec2 BBOX_MIN = glm::vec2(- 500.0f, -25.0f);
static const float ZOOM_SPEED = 1.1f;

namespace Demo
{

    bool Demo06_Scene::init()
	{
		const int shapesCount = m_guiWindow->getNumberOfShapes();
		m_perShapeTypeCount = shapesCount / 5;

		createBbox();
		createCameras();
		createShapes();

        return true;
	}

	void Demo06_Scene::update(double dt)
	{
		if (m_guiWindow != nullptr)
		{
			const int shapesCount = m_guiWindow->getNumberOfShapes();
			m_perShapeTypeCount = shapesCount / 5;
		}
	}

	void Demo06_Scene::render()
	{
		PekanRenderer::clear();

		Camera2D& camera = (m_currentCameraIdx == 0) ? *m_cameraFirst : *m_cameraSecond;

		for (int i = 0; i < m_perShapeTypeCount; i++)
		{
			m_rectangles[i].render(camera);
		}

		m_centerSquare.render(camera);
	}

	void Demo06_Scene::exit()
	{
		for (int i = 0; i < m_rectangles.size(); i++)
		{
			m_rectangles[i].destroy();
		}
		m_centerSquare.destroy();
	}

	void Demo06_Scene::createBbox()
	{
		const glm::vec2 windowSize = PekanEngine::getWindowSize();

		m_bbox.min = BBOX_MIN;
		m_bbox.max =
		{
			m_bbox.min.x + windowSize.x,
			m_bbox.min.y + windowSize.y
		};
		m_bbox.size = m_bbox.max - m_bbox.min;
	}

	void Demo06_Scene::createCameras()
	{
		m_cameraFirst = std::make_shared<Camera2D>();
		m_cameraFirst->setSize(m_bbox.size.x, m_bbox.size.y);
		m_cameraFirst->setPosition(m_bbox.min + m_bbox.size / 2.0f);

		m_cameraSecond = std::make_shared<Camera2D>();
		m_cameraSecond->setSize(m_bbox.size.x, m_bbox.size.y);
		m_cameraSecond->setPosition(m_bbox.min + m_bbox.size / 2.0f);

		PekanTools::enableCameraController2D(m_cameraFirst);
	}

	void Demo06_Scene::createShapes()
	{
		createRectangles();

		m_centerSquare.create(100.0f, 100.0f);
		m_centerSquare.setPosition(m_bbox.min + m_bbox.size / 2.0f - glm::vec2(50.0f, 50.0f));
		m_centerSquare.setColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	}

	void Demo06_Scene::createRectangles()
	{
		const float minWidthHeight = std::min(m_bbox.size.x, m_bbox.size.y);
		const glm::vec2 widthHeightRange =
		{
			minWidthHeight * 0.02f,
			minWidthHeight * 0.08f
		};

		const glm::vec2 positionXRange = { m_bbox.min.x, m_bbox.max.x - widthHeightRange.y };
		const glm::vec2 positionYRange = { m_bbox.min.y, m_bbox.max.y - widthHeightRange.y };

		m_rectangles.resize(m_perShapeTypeCount);

		for (int i = 0; i < m_perShapeTypeCount; i++)
		{
			m_rectangles[i].create(getRandomFloat(widthHeightRange), getRandomFloat(widthHeightRange));
			m_rectangles[i].setPosition(getRandomVec2(positionXRange, positionYRange));
			m_rectangles[i].setColor(getRandomColor());
		}
	}

	bool Demo06_Scene::onKeyPressed(const Pekan::KeyPressedEvent& event)
	{
		if (event.getKeyCode() == KeyCode::KEY_C)
		{
			m_currentCameraIdx = (m_currentCameraIdx + 1) % 2;
			if (m_currentCameraIdx == 0)
			{
				PekanTools::enableCameraController2D(m_cameraFirst);
			}
			else
			{
				PekanTools::enableCameraController2D(m_cameraSecond);
			}
			return true;
		}
		return false;
	}

} // namespace Demo