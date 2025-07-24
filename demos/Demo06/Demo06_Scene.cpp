#include "Demo06_Scene.h"
#include "PekanLogger.h"
#include "Utils/PekanUtils.h"
#include "PekanTools.h"
#include "RenderCommands.h"
#include "Renderer2DSystem.h"

#include "Events/MouseEvents.h"
#include "Events/KeyEvents.h"

#include <algorithm>

static glm::vec2 BBOX_MIN = glm::vec2(- 500.0f, -25.0f);
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

	// Generates random vertices forming a normal polygon.
	// A "normal" polygon is a polygon whose sides do NOT intersect. It could be either convex or concave.
	static std::vector<glm::vec2> getRandomPolygonVertices(int numPoints, glm::vec2 xRange, glm::vec2 yRange)
	{
		std::vector<glm::vec2> points;

		// Generate random points in the given XY range
		for (int i = 0; i < numPoints; ++i)
		{
			points.emplace_back(getRandomVec2(xRange, yRange));
		}

		// Compute centroid (average point) of points
		glm::vec2 centroid(0.0f, 0.0f);
		for (const glm::vec2& p : points)
		{
			centroid += p;
		}
		centroid /= float(points.size());

		// Sort points by angle around centroid
		std::sort(points.begin(), points.end(), [&](const glm::vec2& a, const glm::vec2& b)
			{
				const float angleA = std::atan2(a.y - centroid.y, a.x - centroid.x);
				const float angleB = std::atan2(b.y - centroid.y, b.x - centroid.x);
				return angleA < angleB;
			}
		);

		return points;
	}

    bool Demo06_Scene::init()
	{
		RenderState::enableMultisampleAntiAliasing();

		if (m_guiWindow == nullptr)
		{
			PK_LOG_ERROR("Cannot initialize Demo06_Scene because there is no GUI window attached.", "Demo06");
			return false;
		}

		// We have to manually initialize GUI window before retrieving values from it.
		// In the layer stack the GUI window is AFTER the scene because it needs to be drawn on top,
		// so it will be automatically initialized AFTER the scene, which is not good. We need some of its values here.
		// That's why we will manually initialize the GUI window here.
		// Then Pekan will try to initialize it again, but that's fine, it will just skip.
		m_guiWindow->init();

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

		updateShapes(float(dt));

		t += float(dt);
	}

	void Demo06_Scene::render() const
	{
		Renderer2DSystem::beginFrame();
		RenderCommands::clear();

		if (m_guiWindow->isEnabledRectangles())
		{
			for (int i = 0; i < m_perShapeTypeCount; i++)
			{
				m_rectangles[i].render();
			}
		}

		if (m_guiWindow->isEnabledCircles())
		{
			for (int i = 0; i < m_perShapeTypeCount / 2; i++)
			{
				m_circles[i].render();
			}
		}

		if (m_guiWindow->isEnabledCirclesStatic())
		{
			for (int i = 0; i < m_perShapeTypeCount / 2; i++)
			{
				m_circlesStatic[i].render();
			}
		}

		if (m_guiWindow->isEnabledTriangles())
		{
			for (int i = 0; i < m_perShapeTypeCount; i++)
			{
				m_triangles[i].render();
			}
		}

		if (m_guiWindow->isEnabledPolygons())
		{
			for (int i = 0; i < m_perShapeTypeCount; i++)
			{
				m_polygons[i].render();
			}
		}

		if (m_guiWindow->isEnabledLines())
		{
			for (int i = 0; i < m_perShapeTypeCount; i++)
			{
				m_lines[i].render();
			}
		}

		m_centerSquare.render();

		Renderer2DSystem::endFrame();
	}

	void Demo06_Scene::exit()
	{
		for (int i = 0; i < m_rectangles.size(); i++)
		{
			m_rectangles[i].destroy();
		}
		for (int i = 0; i < m_circles.size(); i++)
		{
			m_circles[i].destroy();
		}
		for (int i = 0; i < m_circlesStatic.size(); i++)
		{
			m_circlesStatic[i].destroy();
		}
		for (int i = 0; i < m_triangles.size(); i++)
		{
			m_triangles[i].destroy();
		}
		for (int i = 0; i < m_polygons.size(); i++)
		{
			m_polygons[i].destroy();
		}
		for (int i = 0; i < m_lines.size(); i++)
		{
			m_lines[i].destroy();
		}
		m_centerSquare.destroy();
	}

	void Demo06_Scene::createBbox()
	{
		const glm::vec2 windowSize = PekanEngine::getWindow().getSize();

		m_bbox.min = BBOX_MIN;
		m_bbox.max =
		{
			m_bbox.min.x + windowSize.x,
			m_bbox.min.y + windowSize.y
		};
		m_bbox.size = m_bbox.max - m_bbox.min;
		m_bbox.center = m_bbox.min + m_bbox.size / 2.0f;
	}

	void Demo06_Scene::createCameras()
	{
		m_cameraFirst = std::make_shared<Camera2D>();
		m_cameraFirst->setSize(m_bbox.size.x, m_bbox.size.y);
		m_cameraFirst->setPosition(m_bbox.center);

		m_cameraSecond = std::make_shared<Camera2D>();
		m_cameraSecond->setSize(m_bbox.size.x, m_bbox.size.y);
		m_cameraSecond->setPosition(m_bbox.center);

		Renderer2DSystem::setCamera(m_cameraFirst);
		PekanTools::enableCameraController2D(m_cameraFirst);
		PekanTools::setCameraController2DZoomSpeed(1.1f);
	}

	void Demo06_Scene::createShapes()
	{
		createRectangles();
		createCircles();
		createCirclesStatic();
		createTriangles();
		createPolygons();
		createLines();
		createCenterSquare();
	}

	void Demo06_Scene::createRectangles()
	{
		const float minDim = std::min(m_bbox.size.x, m_bbox.size.y);
		const glm::vec2 widthHeightRange =
		{
			minDim * 0.001f,
			minDim * 0.005f
		};
		const glm::vec2 positionXRange = { m_bbox.min.x + widthHeightRange.y / 2.0f, m_bbox.max.x - widthHeightRange.y / 2.0f };
		const glm::vec2 positionYRange = { m_bbox.min.y + widthHeightRange.y / 2.0f, m_bbox.max.y - widthHeightRange.y / 2.0f };

		m_rectangles.resize(m_perShapeTypeCount);

		for (int i = 0; i < m_perShapeTypeCount; i++)
		{
			m_rectangles[i].create(getRandomFloat(widthHeightRange), getRandomFloat(widthHeightRange));
			m_rectangles[i].setPosition(getRandomVec2(positionXRange, positionYRange));
			m_rectangles[i].setColor(getRandomColor(0.1f, 0.9f));
		}
	}

	void Demo06_Scene::createCircles()
	{
		const float minDim = std::min(m_bbox.size.x, m_bbox.size.y);
		const glm::vec2 radiusRange = { minDim * 0.001f, minDim * 0.006f };
		const glm::vec2 positionXRange = { m_bbox.min.x + radiusRange.y, m_bbox.max.x - radiusRange.y };
		const glm::vec2 positionYRange = { m_bbox.min.y + radiusRange.y, m_bbox.max.y - radiusRange.y };

		m_circles.resize(m_perShapeTypeCount / 2);

		for (int i = 0; i < m_perShapeTypeCount / 2; i++)
		{
			m_circles[i].create(getRandomFloat(radiusRange));
			m_circles[i].setPosition(getRandomVec2(positionXRange, positionYRange));
			m_circles[i].setColor(getRandomColor(0.1f, 0.9f));
			m_circles[i].setSegmentsCount(getRandomInt({ 8, 60 }));
		}
	}

	void Demo06_Scene::createCirclesStatic()
	{
		const float minDim = std::min(m_bbox.size.x, m_bbox.size.y);
		const glm::vec2 radiusRange = { minDim * 0.001f, minDim * 0.006f };
		const glm::vec2 positionXRange = { m_bbox.min.x + radiusRange.y, m_bbox.max.x - radiusRange.y };
		const glm::vec2 positionYRange = { m_bbox.min.y + radiusRange.y, m_bbox.max.y - radiusRange.y };

		m_circlesStatic.resize(m_perShapeTypeCount / 2);

		for (int i = 0; i < m_perShapeTypeCount / 2; i++)
		{
			m_circlesStatic[i].create(getRandomFloat(radiusRange));
			m_circlesStatic[i].setPosition(getRandomVec2(positionXRange, positionYRange));
			m_circlesStatic[i].setColor(getRandomColor(0.1f, 0.9f));
		}
	}

	void Demo06_Scene::createTriangles()
	{
		const float minDim = std::min(m_bbox.size.x, m_bbox.size.y);
		const glm::vec2 pointXRange ={ -minDim * 0.003f, minDim * 0.003f };
		const glm::vec2 pointYRange = { -minDim * 0.003f, minDim * 0.003f };
		const glm::vec2 positionXRange = { m_bbox.min.x - pointXRange.x, m_bbox.max.x - pointXRange.y };
		const glm::vec2 positionYRange = { m_bbox.min.y - pointYRange.x, m_bbox.max.y - pointYRange.y };

		m_triangles.resize(m_perShapeTypeCount);

		for (int i = 0; i < m_perShapeTypeCount; i++)
		{
			m_triangles[i].create
			(
				getRandomVec2(pointXRange, pointYRange),
				getRandomVec2(pointXRange, pointYRange),
				getRandomVec2(pointXRange, pointYRange)
			);
			m_triangles[i].setPosition(getRandomVec2(positionXRange, positionYRange));
			m_triangles[i].setColor(getRandomColor(0.1f, 0.9f));
		}
	}

	void Demo06_Scene::createPolygons()
	{
		const float minDim = std::min(m_bbox.size.x, m_bbox.size.y);
		const glm::vec2 pointXRange = { -minDim * 0.0025f, minDim * 0.0025f };
		const glm::vec2 pointYRange = { -minDim * 0.0025f, minDim * 0.0025f };
		const glm::vec2 positionXRange = { m_bbox.min.x - pointXRange.x, m_bbox.max.x - pointXRange.y };
		const glm::vec2 positionYRange = { m_bbox.min.y + pointYRange.y, m_bbox.max.y - pointYRange.y };

		m_polygons.resize(m_perShapeTypeCount);
		for (int i = 0; i < m_perShapeTypeCount; i++)
		{
			const std::vector<glm::vec2> vertices = getRandomPolygonVertices(getRandomInt(6, 18), pointXRange, pointYRange);
			m_polygons[i].create(vertices);
			m_polygons[i].setPosition(getRandomVec2(positionXRange, positionYRange));
			m_polygons[i].setColor(getRandomColor(0.1f, 0.9f));
		}
	}

	void Demo06_Scene::createLines()
	{
		const float minDim = std::min(m_bbox.size.x, m_bbox.size.y);
		const glm::vec2 pointXRange = { -minDim * 0.004f, minDim * 0.004f };
		const glm::vec2 pointYRange = { -minDim * 0.004f, minDim * 0.004f };
		const glm::vec2 positionXRange = { m_bbox.min.x - pointXRange.x, m_bbox.max.x - pointXRange.y };
		const glm::vec2 positionYRange = { m_bbox.min.y - pointYRange.x, m_bbox.max.y - pointYRange.y };

		m_lines.resize(m_perShapeTypeCount);

		for (int i = 0; i < m_perShapeTypeCount; i++)
		{
			m_lines[i].create
			(
				getRandomVec2(pointXRange, pointYRange),
				getRandomVec2(pointXRange, pointYRange),
				getRandomFloat(0.1f, 0.6f)
			);
			m_lines[i].setPosition(getRandomVec2(positionXRange, positionYRange));
			m_lines[i].setColor(getRandomColor(0.3f, 0.65f));
		}
	}

	void Demo06_Scene::createCenterSquare()
	{
		m_centerSquare.create(100.0f, 100.0f, false);
		m_centerSquare.setPosition(m_bbox.center);
		m_centerSquare.setColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	}

	void Demo06_Scene::updateShapes(float dt)
	{

		if (m_guiWindow->isEnabledRectangles())
		{
			updateRectangles(dt);
		}

		if (m_guiWindow->isEnabledCircles())
		{
			updateCircles(dt);
		}

		if (m_guiWindow->isEnabledCirclesStatic())
		{
			updateCirclesStatic(dt);
		}

		if (m_guiWindow->isEnabledTriangles())
		{
			updateTriangles(dt);
		}

		if (m_guiWindow->isEnabledPolygons())
		{
			updatePolygons(dt);
		}

		if (m_guiWindow->isEnabledLines())
		{
			updateLines(dt);
		}
	}

	void Demo06_Scene::updateRectangles(float dt)
	{
		for (int i = 0; i < m_perShapeTypeCount; i++)
		{
			m_rectangles[i].move(getRandomVec2
			(
				{ -float((i * 4 + 2) % 30) * 1.0f, float((i * 3 + 11) % 30) * 1.0f },
				{ -float((i * 7 + 6) % 30) * 1.0f, float((i * 11 + 3) % 30) * 1.0f }
			) * dt);
			m_rectangles[i].setRotation(dt * sin(t * float(i % 7)) * float(i % 17) / 3.5f);
			m_rectangles[i].setScale(m_rectangles[i].getScale() * getRandomVec2
			(
				{ 0.96f, 1.04f },
				{ 0.96f, 1.04f }
			));
		}
	}

	void Demo06_Scene::updateCircles(float dt)
	{
		for (int i = 0; i < m_perShapeTypeCount / 2; i++)
		{
			m_circles[i].move(getRandomVec2
			(
				{ -float((i * 7 + 1) % 30), float((i * 7 + 6) % 30) },
				{ -float((i * 7 + 2) % 30), float((i * 13 + 4) % 30) }
			) * dt);
			m_circles[i].setRotation(dt * sin(t * float(i % 5)) * float(i % 17 + 7) / 3.0f);
			m_circles[i].setScale(m_circles[i].getScale() * getRandomVec2
			(
				{ 0.96f, 1.04f },
				{ 0.96f, 1.04f }
			));
		}
	}

	void Demo06_Scene::updateCirclesStatic(float dt)
	{
		for (int i = 0; i < m_perShapeTypeCount / 2; i++)
		{
			m_circlesStatic[i].move(getRandomVec2
			(
				{ -float((i * 7 + 1) % 30), float((i * 7 + 6) % 30) },
				{ -float((i * 7 + 2) % 30), float((i * 13 + 4) % 30) }
			) * dt);
			m_circlesStatic[i].setRotation(dt * sin(t * float(i % 3)) * float(i % 17 + 7) / 3.0f);
			m_circlesStatic[i].setScale(m_circles[i].getScale() * getRandomVec2
			(
				{ 0.96f, 1.04f },
				{ 0.96f, 1.04f }
			));
		}
	}

	void Demo06_Scene::updateTriangles(float dt)
	{
		for (int i = 0; i < m_perShapeTypeCount; i++)
		{
			m_triangles[i].move(getRandomVec2
			(
				{ -float((i * 7 + 2) % 30), float((i * 3 + 7) % 30) },
				{ -float((i * 11 + 3) % 30), float((i * 33 + 17) % 30) }
			) * dt);
			m_triangles[i].setRotation(dt * sin(t * float(i % 9)) * float(i % 19 + 5) / 4.5f);
			m_triangles[i].setScale(m_triangles[i].getScale() * getRandomVec2
			(
				{ 0.96f, 1.04f },
				{ 0.96f, 1.04f }
			));
		}
	}

	void Demo06_Scene::updatePolygons(float dt)
	{
		for (int i = 0; i < m_perShapeTypeCount; i++)
		{
			m_polygons[i].move(getRandomVec2
			(
				{ -float((i * 12 + 5) % 30), float((i * 11 + 6) % 30) },
				{ -float((i * 9 + 2) % 30), float((i * 19 + 4) % 30) }
			) * dt);
			m_polygons[i].setRotation(dt * sin(t * float(i % 8)) * float(i % 23 + 3) / 3.0f);
			m_polygons[i].setScale(m_polygons[i].getScale() * getRandomVec2
			(
				{ 0.96f, 1.04f },
				{ 0.96f, 1.04f }
			));
		}
	}

	void Demo06_Scene::updateLines(float dt)
	{
		for (int i = 0; i < m_perShapeTypeCount; i++)
		{
			m_lines[i].move(getRandomVec2
			(
				{ -float((i * 5 + 2) % 30), float((i * 17 + 11) % 30) },
				{ -float((i * 7 + 3) % 30), float((i * 15 + 9) % 30) }
			) * dt);
			m_lines[i].setRotation(dt * sin(t * float(i % 9)) * float(i % 19 + 5) / 5.5f);
			m_lines[i].setScale(m_lines[i].getScale() * getRandomVec2
			(
				{ 0.98f, 1.02f },
				{ 0.98f, 1.02f }
			));
		}
	}

	bool Demo06_Scene::onKeyPressed(const Pekan::KeyPressedEvent& event)
	{
		if (event.getKeyCode() == KeyCode::KEY_C)
		{
			m_currentCameraIdx = (m_currentCameraIdx + 1) % 2;
			if (m_currentCameraIdx == 0)
			{
				Renderer2DSystem::setCamera(m_cameraFirst);
				PekanTools::enableCameraController2D(m_cameraFirst);
			}
			else
			{
				Renderer2DSystem::setCamera(m_cameraSecond);
				PekanTools::enableCameraController2D(m_cameraSecond);
			}
			return true;
		}
		return false;
	}

} // namespace Demo