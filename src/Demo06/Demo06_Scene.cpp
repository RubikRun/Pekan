#include "Demo06_Scene.h"
#include "Logger/PekanLogger.h"
#include "Utils/PekanUtils.h"

using Pekan::PekanEngine;
using Pekan::Renderer::PekanRenderer;
using Pekan::Renderer::ShaderDataType;
using Pekan::Renderer::DrawMode;
using Pekan::Renderer::BufferDataUsage;
using Pekan::Renderer::Shader;
using Pekan::Renderer::RectangleShape;
using Pekan::Renderer::CircleShape;
using Pekan::Renderer::CircleShapeStatic;
using Pekan::Renderer::TriangleShape;
using Pekan::Renderer::PolygonShape;
using Pekan::Renderer::LineShape;
using Pekan::Renderer::Camera2D;
using Pekan::Utils::getRandomFloat;
using Pekan::Utils::getRandomVec2;
using Pekan::Utils::getRandomColor;

static const char* vertexShaderFilePath = "resources/06_vertex_shader.glsl";
static const char* fragmentShaderFilePath = "resources/06_fragment_shader.glsl";

static glm::vec2 BBOX_MIN = glm::vec2(- 500.0f, -25.0f);

namespace Demo
{

    bool Demo06_Scene::init()
	{
		const int shapesCount = m_guiWindow->getNumberOfShapes();
		m_perShapeTypeCount = shapesCount / 5;

		createBbox();
		createCamera();
		createShapes();

        return true;
	}

	void Demo06_Scene::update(double dt)
	{
		const int shapesCount = m_guiWindow->getNumberOfShapes();
		m_perShapeTypeCount = shapesCount / 5;

		m_camera.move({ 0.2f, 0.3f });
		m_camera.zoomOut(1.0005f);
	}

	void Demo06_Scene::render()
	{
		PekanRenderer::clear();

		for (int i = 0; i < m_perShapeTypeCount; i++)
		{
			m_rectangles[i].render(m_camera);
		}
	}

	void Demo06_Scene::exit()
	{
		for (int i = 0; i < m_rectangles.size(); i++)
		{
			m_rectangles[i].destroy();
		}
	}

	void Demo06_Scene::createBbox()
	{
		const glm::vec2 windowResolution = PekanEngine::getWindowResolution();

		m_bbox.min = BBOX_MIN;
		m_bbox.max =
		{
			m_bbox.min.x + windowResolution.x,
			m_bbox.min.y + windowResolution.y
		};
		m_bbox.size = m_bbox.max - m_bbox.min;
	}

	void Demo06_Scene::createCamera()
	{
		m_camera.setSize(m_bbox.size.x, m_bbox.size.y);
		m_camera.setPosition(m_bbox.min + (m_bbox.max - m_bbox.min) / 2.0f);
	}

	void Demo06_Scene::createShapes()
	{
		if (m_guiWindow == nullptr)
		{
			PK_LOG_ERROR("GUI window is null", "Demo06");
		}

		createRectangles();
	}

	void Demo06_Scene::createRectangles()
	{
		const glm::vec2 widthRange =
		{
			m_bbox.size.x * 0.02f,
			m_bbox.size.x * 0.08f
		};
		const glm::vec2 heightRange =
		{
			m_bbox.size.y * 0.02f,
			m_bbox.size.y * 0.08f
		};
		const glm::vec2 positionXRange = { m_bbox.min.x, m_bbox.max.x - widthRange.y };
		const glm::vec2 positionYRange = { m_bbox.min.y, m_bbox.max.y - heightRange.y };

		m_rectangles.resize(m_perShapeTypeCount);

		for (int i = 0; i < m_perShapeTypeCount; i++)
		{
			m_rectangles[i].create(getRandomFloat(widthRange), getRandomFloat(heightRange));
			m_rectangles[i].setPosition(getRandomVec2(positionXRange, positionYRange));
			m_rectangles[i].setColor(getRandomColor());
		}
	}

} // namespace Demo