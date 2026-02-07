#include "Demo06_Scene.h"

#include "Demo06_GUIWindow.h"
#include "PekanLogger.h"
#include "Utils/RandomizationUtils.h"
#include "RenderState.h"
#include "PostProcessor.h"
#include "Shader.h"
#include "ShaderPreprocessor.h"

#include "CameraComponent2D.h"
#include "TransformComponent2D.h"
#include "RectangleGeometryComponent.h"
#include "CircleGeometryComponent.h"
#include "TriangleGeometryComponent.h"
#include "PolygonGeometryComponent.h"
#include "LineGeometryComponent.h"
#include "SolidColorMaterialComponent.h"

#include "Events/KeyEvents.h"

#include <algorithm>

#define POST_PROCESSING_SHADER_FILEPATH_PKSHAD "Shaders/PostProcessingShader.pkshad"
#define POST_PROCESSING_SHADER_FILEPATH_GLSL "Shaders/PostProcessingShader.glsl"

using namespace Pekan;
using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;
using namespace Pekan::RandomizationUtils;

static glm::vec2 BBOX_MIN = glm::vec2(-500.0f, -25.0f);
constexpr float ZOOM_SPEED = 1.1f;

// Returns a random color for a shape
static glm::vec4 generateShapeColor()
{
	return getRandomColor
	(
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.9f, 0.9f, 0.9f)
	);
}

constexpr float kernelIdentity[9] = {
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f
};

static void lerpKernels(const float* a, const float* b, float* out, float w)
{
	for (int i = 0; i < 9; i++)
	{
		out[i] = (1.0f - w) * a[i] + w * b[i];
	}
}

static void getKernelSharpen(float* kernel, float t)
{
	float w = 0.5f * (sinf(t * 2.0f) + 1.0f);
	static constexpr float base[9] = {
		 0.0f, -1.0f,  0.0f,
		-1.0f,  5.0f, -1.0f,
		 0.0f, -1.0f,  0.0f
	};
	lerpKernels(kernelIdentity, base, kernel, w);
}

static void getKernelBlur(float* kernel, float t)
{
	float w = 0.5f * (sinf(t * 2.0f) + 1.0f);
	static constexpr float base[9] = {
		1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f,
		1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f,
		1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f
	};
	lerpKernels(kernelIdentity, base, kernel, w);
}

static void getKernelEdgeDetection(float* kernel, float t)
{
	float w = 0.5f * (sinf(t * 2.0f) + 1.0f);
	static constexpr float base[9] = {
		 1.0f,  1.0f,  1.0f,
		 1.0f, -8.0f,  1.0f,
		 1.0f,  1.0f,  1.0f
	};
	lerpKernels(kernelIdentity, base, kernel, w);
}

static void getKernelEmboss(float* kernel, float t)
{
	float w = 0.5f * (sinf(t * 2.0f) + 1.0f);
	static constexpr float base[9] = {
		-2.0f, -1.0f,  0.0f,
		-1.0f,  1.0f,  1.0f,
		 0.0f,  1.0f,  2.0f
	};
	lerpKernels(kernelIdentity, base, kernel, w);
}

namespace Demo
{

	// Generates random vertices forming a normal polygon.
	// A "normal" polygon is a polygon whose sides do NOT intersect. It could be either convex or concave.
	static std::vector<glm::vec2> getRandomPolygonVertices(int numPoints, glm::vec2 min, glm::vec2 max)
	{
		std::vector<glm::vec2> points;

		// Generate random points in the given XY range
		for (int i = 0; i < numPoints; ++i)
		{
			points.emplace_back(getRandomVec2(min, max));
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

	bool Demo06_Scene::_init()
	{
		RenderState::enableMultisampleAntiAliasing();

		if (m_guiWindow == nullptr)
		{
			PK_LOG_ERROR("Cannot initialize Demo06_Scene because there is no GUI window attached.", "Demo06");
			return false;
		}

		const int shapesCount = m_guiWindow->getNumberOfShapes();
		const int shapesMaxCount = m_guiWindow->getMaxNumberOfShapes();
		m_perShapeTypeCount = shapesCount / 5;
		m_prevPerShapeTypeCount = m_perShapeTypeCount;
		m_perShapeTypeMaxCount = shapesMaxCount / 5;

		createBbox();
		createCameras();
		createShapes();

		ShaderPreprocessor::preprocess
		(
			POST_PROCESSING_SHADER_FILEPATH_PKSHAD,
			{
				{ "NEIGHBORS_SAMPLE_OFFSET_INVERSE_X", std::to_string(PekanEngine::getWindow().getSize().x / 3) },
				{ "NEIGHBORS_SAMPLE_OFFSET_INVERSE_Y", std::to_string(PekanEngine::getWindow().getSize().y / 3) }
			}
		);

		PostProcessor::setPostProcessingShader(POST_PROCESSING_SHADER_FILEPATH_GLSL);

		// Initialize previous enabled states of shape types,
		// set them to opposite of current states so that in the first update() call,
		// we'll register a change and react accordingly.
		m_prevRectanglesEnabled = !m_guiWindow->isEnabledRectangles();
		m_prevCirclesEnabled = !m_guiWindow->isEnabledCircles();
		m_prevTrianglesEnabled = !m_guiWindow->isEnabledTriangles();
		m_prevPolygonsEnabled = !m_guiWindow->isEnabledPolygons();
		m_prevLinesEnabled = !m_guiWindow->isEnabledLines();

		return true;
	}

	void Demo06_Scene::_exit()
	{
		destroyEntity(m_centerSquare);
		for (int i = 0; i < m_lines.size(); i++)
		{
			destroyEntity(m_lines[i]);
		}
		for (int i = 0; i < m_polygons.size(); i++)
		{
			destroyEntity(m_polygons[i]);
		}
		for (int i = 0; i < m_triangles.size(); i++)
		{
			destroyEntity(m_triangles[i]);
		}
		for (int i = 0; i < m_circles.size(); i++)
		{
			destroyEntity(m_circles[i]);
		}
		for (int i = 0; i < m_rectangles.size(); i++)
		{
			destroyEntity(m_rectangles[i]);
		}
	}

	void Demo06_Scene::update(double dt)
	{
		PK_ASSERT_QUICK(m_guiWindow != nullptr);

		const int shapesCount = m_guiWindow->getNumberOfShapes();
		m_perShapeTypeCount = shapesCount / 5;
		const bool perShapeTypeCountChanged = (m_perShapeTypeCount != m_prevPerShapeTypeCount);
		m_prevPerShapeTypeCount = m_perShapeTypeCount;

		updateShapes(float(dt), perShapeTypeCountChanged);
		updatePps();

		t += float(dt);
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
		m_cameraFirst = createEntity();
		CameraComponent2D cameraComponentFirst;
		cameraComponentFirst.size = m_bbox.size;
		cameraComponentFirst.position = m_bbox.center;
		m_registry.emplace<CameraComponent2D>(m_cameraFirst, cameraComponentFirst);

		m_cameraSecond = createEntity();
		CameraComponent2D cameraComponentSecond;
		cameraComponentSecond.size = m_bbox.size;
		cameraComponentSecond.position = m_bbox.center;
		cameraComponentSecond.isPrimary = false;
		cameraComponentSecond.isControllable = false;
		m_registry.emplace<CameraComponent2D>(m_cameraSecond, cameraComponentSecond);
	}

	void Demo06_Scene::createShapes()
	{
		createRectangles();
		createCircles();
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
		const glm::vec2 positionMin = { m_bbox.min.x + widthHeightRange.y / 2.0f, m_bbox.min.y + widthHeightRange.y / 2.0f };
		const glm::vec2 positionMax = { m_bbox.max.x - widthHeightRange.y / 2.0f, m_bbox.max.y - widthHeightRange.y / 2.0f };

		m_rectangles.resize(m_perShapeTypeMaxCount);
		for (int i = 0; i < m_perShapeTypeMaxCount; i++)
		{
			m_rectangles[i] = createEntity();
			m_registry.emplace<TransformComponent2D>(
				m_rectangles[i],
				getRandomVec2(positionMin, positionMax)
			);
			m_registry.emplace<RectangleGeometryComponent>(
				m_rectangles[i],
				getRandomFloat(widthHeightRange.x, widthHeightRange.y),
				getRandomFloat(widthHeightRange.x, widthHeightRange.y)
			);
			m_registry.emplace<SolidColorMaterialComponent>(
				m_rectangles[i],
				generateShapeColor()
			);
		}
	}

	void Demo06_Scene::createCircles()
	{
		const float minDim = std::min(m_bbox.size.x, m_bbox.size.y);
		const glm::vec2 radiusRange = { minDim * 0.001f, minDim * 0.006f };
		const glm::vec2 positionMin = { m_bbox.min.x + radiusRange.y, m_bbox.min.y + radiusRange.y };
		const glm::vec2 positionMax = { m_bbox.max.x - radiusRange.y, m_bbox.max.y - radiusRange.y };

		m_circles.resize(m_perShapeTypeMaxCount);

		for (int i = 0; i < m_perShapeTypeMaxCount; i++)
		{
			m_circles[i] = createEntity();
			m_registry.emplace<TransformComponent2D>(
				m_circles[i],
				getRandomVec2(positionMin, positionMax)
			);
			m_registry.emplace<CircleGeometryComponent>(
				m_circles[i],
				getRandomFloat(radiusRange.x, radiusRange.y),
				getRandomInt(8, 60)
			);
			m_registry.emplace<SolidColorMaterialComponent>(
				m_circles[i],
				generateShapeColor()
			);
		}
	}

	void Demo06_Scene::createTriangles()
	{
		const float minDim = std::min(m_bbox.size.x, m_bbox.size.y);
		const glm::vec2 pointMin ={ -minDim * 0.003f, -minDim * 0.003f };
		const glm::vec2 pointMax = { minDim * 0.003f, minDim * 0.003f };
		const glm::vec2 positionMin = { m_bbox.min.x - pointMin.x, m_bbox.min.y - pointMin.y };
		const glm::vec2 positionMax = { m_bbox.max.x - pointMax.x, m_bbox.max.y - pointMax.y };

		m_triangles.resize(m_perShapeTypeMaxCount);

		for (int i = 0; i < m_perShapeTypeMaxCount; i++)
		{
			m_triangles[i] = createEntity();
			m_registry.emplace<TransformComponent2D>(
				m_triangles[i],
				getRandomVec2(positionMin, positionMax)
			);
			m_registry.emplace<TriangleGeometryComponent>(
				m_triangles[i],
				getRandomVec2(pointMin, pointMax),
				getRandomVec2(pointMin, pointMax),
				getRandomVec2(pointMin, pointMax)
			);
			m_registry.emplace<SolidColorMaterialComponent>(
				m_triangles[i],
				generateShapeColor()
			);
		}
	}

	void Demo06_Scene::createPolygons()
	{
		const float minDim = std::min(m_bbox.size.x, m_bbox.size.y);
		const glm::vec2 pointMin = { -minDim * 0.0025f, -minDim * 0.0025f };
		const glm::vec2 pointMax = { minDim * 0.0025f, minDim * 0.0025f };
		const glm::vec2 positionMin = { m_bbox.min.x - pointMin.x, m_bbox.min.y - pointMin.y };
		const glm::vec2 positionMax = { m_bbox.max.x - pointMax.x, m_bbox.max.y - pointMax.y };

		m_polygons.resize(m_perShapeTypeMaxCount);
		for (int i = 0; i < m_perShapeTypeMaxCount; i++)
		{
			const std::vector<glm::vec2> vertices = getRandomPolygonVertices(getRandomInt(6, 18), pointMin, pointMax);
			m_polygons[i] = createEntity();
			m_registry.emplace<TransformComponent2D>(
				m_polygons[i],
				getRandomVec2(positionMin, positionMax)
			);
			m_registry.emplace<PolygonGeometryComponent>(
				m_polygons[i],
				vertices
			);
			m_registry.emplace<SolidColorMaterialComponent>(
				m_polygons[i],
				generateShapeColor()
			);
		}
	}

	void Demo06_Scene::createLines()
	{
		const float minDim = std::min(m_bbox.size.x, m_bbox.size.y);
		const glm::vec2 pointMin = { -minDim * 0.004f, -minDim * 0.004f };
		const glm::vec2 pointMax = { minDim * 0.004f, minDim * 0.004f };
		const glm::vec2 positionMin = { m_bbox.min.x - pointMin.x, m_bbox.min.y - pointMin.y };
		const glm::vec2 positionMax = { m_bbox.max.x - pointMax.x, m_bbox.max.y - pointMax.y };

		m_lines.resize(m_perShapeTypeMaxCount);

		for (int i = 0; i < m_perShapeTypeMaxCount; i++)
		{
			m_lines[i] = createEntity();
			m_registry.emplace<TransformComponent2D>(
				m_lines[i],
				getRandomVec2(positionMin, positionMax)
			);
			m_registry.emplace<LineGeometryComponent>(
				m_lines[i],
				getRandomVec2(pointMin, pointMax),
				getRandomVec2(pointMin, pointMax),
				getRandomFloat(0.1f, 0.6f)
			);
			m_registry.emplace<SolidColorMaterialComponent>(
				m_lines[i],
				generateShapeColor()
			);
		}
	}

	void Demo06_Scene::createCenterSquare()
	{
		m_centerSquare = createEntity();
		m_registry.emplace<TransformComponent2D>(
			m_centerSquare,
			m_bbox.center
		);
		m_registry.emplace<RectangleGeometryComponent>(
			m_centerSquare,
			100.0f, 100.0f
		);
		m_registry.emplace<SolidColorMaterialComponent>(
			m_centerSquare,
			glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f }
		);
	}

	void Demo06_Scene::updateShapes(float dt, bool perShapeTypeCountChanged)
	{
		const bool rectanglesEnabled = m_guiWindow->isEnabledRectangles();
		// If number of shapes per shape type changed or rectangles enabled state changed,
		// then we need to update disabled/enabled state of rectangle entities.
		if ((perShapeTypeCountChanged && rectanglesEnabled) || rectanglesEnabled != m_prevRectanglesEnabled)
		{
			updateDisabledEnabledStateOfShapeType(m_rectangles, rectanglesEnabled);
		}
		m_prevRectanglesEnabled = rectanglesEnabled;
		if (rectanglesEnabled)
		{
			updateRectangles(dt);
		}

		const bool circlesEnabled = m_guiWindow->isEnabledCircles();
		// If number of shapes per shape type changed or circles enabled state changed,
		// then we need to update disabled/enabled state of circle entities.
		if ((perShapeTypeCountChanged && circlesEnabled) || circlesEnabled != m_prevCirclesEnabled)
		{
			updateDisabledEnabledStateOfShapeType(m_circles, circlesEnabled);
		}
		m_prevCirclesEnabled = circlesEnabled;
		if (circlesEnabled)
		{
			updateCircles(dt);
		}

		const bool trianglesEnabled = m_guiWindow->isEnabledTriangles();
		// If number of shapes per shape type changed or triangles enabled state changed,
		// then we need to update disabled/enabled state of triangle entities.
		if ((perShapeTypeCountChanged && trianglesEnabled) || trianglesEnabled != m_prevTrianglesEnabled)
		{
			updateDisabledEnabledStateOfShapeType(m_triangles, trianglesEnabled);
		}
		m_prevTrianglesEnabled = trianglesEnabled;
		if (trianglesEnabled)
		{
			updateTriangles(dt);
		}

		const bool polygonsEnabled = m_guiWindow->isEnabledPolygons();
		// If number of shapes per shape type changed or polygons enabled state changed,
		// then we need to update disabled/enabled state of polygon entities.
		if ((perShapeTypeCountChanged && polygonsEnabled) || polygonsEnabled != m_prevPolygonsEnabled)
		{
			updateDisabledEnabledStateOfShapeType(m_polygons, polygonsEnabled);
		}
		m_prevPolygonsEnabled = polygonsEnabled;
		if (polygonsEnabled)
		{
			updatePolygons(dt);
		}

		const bool linesEnabled = m_guiWindow->isEnabledLines();
		// If number of shapes per shape type changed or lines enabled state changed,
		// then we need to update disabled/enabled state of line entities.
		if ((perShapeTypeCountChanged && linesEnabled) || linesEnabled != m_prevLinesEnabled)
		{
			updateDisabledEnabledStateOfShapeType(m_lines, linesEnabled);
		}
		m_prevLinesEnabled = linesEnabled;
		if (linesEnabled)
		{
			updateLines(dt);
		}
	}

	void Demo06_Scene::updateDisabledEnabledStateOfShapeType(std::vector<entt::entity>& shapes, bool enabled)
	{
		// If shape type is enabled in GUI,
		// enable entities up to m_perShapeTypeCount and disable the rest.
		if (enabled)
		{
			for (int i = 0; i < m_perShapeTypeCount; i++)
			{
				enableEntity(shapes[i]);
			}
			for (int i = m_perShapeTypeCount; i < m_perShapeTypeMaxCount; i++)
			{
				disableEntity(shapes[i]);
			}
		}
		// If shape type is disabled in GUI,
		// disable all entities of that shape type.
		else
		{
			for (int i = 0; i < m_perShapeTypeMaxCount; i++)
			{
				disableEntity(shapes[i]);
			}
		}
	}

	void Demo06_Scene::updateRectangles(float dt)
	{
		for (int i = 0; i < m_perShapeTypeCount; i++)
		{
			TransformComponent2D& transform = m_registry.get<TransformComponent2D>(m_rectangles[i]);

			transform.move(getRandomVec2
			(
				{ -float((i * 4 + 2) % 30) * 1.0f, -float((i * 7 + 6) % 30) * 1.0f },
				{ float((i * 3 + 11) % 30) * 1.0f, float((i * 11 + 3) % 30) * 1.0f }
			) * dt);
			transform.rotation = dt * sin(t * float(i % 7)) * float(i % 17) / 3.5f;
			transform.scaleFactor *= getRandomVec2
			(
				{ 0.96f, 0.96f },
				{ 1.04f, 1.04f }
			);
		}
	}

	void Demo06_Scene::updateCircles(float dt)
	{
		for (int i = 0; i < m_perShapeTypeCount; i++)
		{
			TransformComponent2D& transform = m_registry.get<TransformComponent2D>(m_circles[i]);

			transform.move(getRandomVec2
			(
				{ -float((i * 7 + 1) % 30), -float((i * 7 + 2) % 30) },
				{ float((i * 7 + 6) % 30), float((i * 13 + 4) % 30) }
			) * dt);
			transform.rotation = dt * sin(t * float(i % 5)) * float(i % 17 + 7) / 3.0f;
			transform.scaleFactor *= getRandomVec2
			(
				{ 0.96f, 0.96f },
				{ 1.04f, 1.04f }
			);
		}
	}

	void Demo06_Scene::updateTriangles(float dt)
	{
		for (int i = 0; i < m_perShapeTypeCount; i++)
		{
			TransformComponent2D& transform = m_registry.get<TransformComponent2D>(m_triangles[i]);

			transform.move(getRandomVec2
			(
				{ -float((i * 7 + 2) % 30), -float((i * 11 + 3) % 30) },
				{ float((i * 3 + 7) % 30), float((i * 33 + 17) % 30) }
			) * dt);
			transform.rotation = dt * sin(t * float(i % 9)) * float(i % 19 + 5) / 4.5f;
			transform.scaleFactor *= getRandomVec2
			(
				{ 0.96f, 0.96f },
				{ 1.04f, 1.04f }
			);
		}
	}

	void Demo06_Scene::updatePolygons(float dt)
	{
		for (int i = 0; i < m_perShapeTypeCount; i++)
		{
			TransformComponent2D& transform = m_registry.get<TransformComponent2D>(m_polygons[i]);

			transform.move(getRandomVec2
			(
				{ -float((i * 12 + 5) % 30), -float((i * 9 + 2) % 30) },
				{ float((i * 11 + 6) % 30), float((i * 19 + 4) % 30) }
			) * dt);
			transform.rotation = dt * sin(t * float(i % 8)) * float(i % 23 + 3) / 3.0f;
			transform.scaleFactor *= getRandomVec2
			(
				{ 0.96f, 0.96f },
				{ 1.04f, 1.04f }
			);
		}
	}

	void Demo06_Scene::updateLines(float dt)
	{
		for (int i = 0; i < m_perShapeTypeCount; i++)
		{
			TransformComponent2D& transform = m_registry.get<TransformComponent2D>(m_lines[i]);

			transform.move(getRandomVec2
			(
				{ -float((i * 5 + 2) % 30), -float((i * 7 + 3) % 30) },
				{ float((i * 17 + 11) % 30), float((i * 15 + 9) % 30) }
			) * dt);
			transform.rotation = dt * sin(t * float(i % 9)) * float(i % 19 + 5) / 5.5f;
			transform.scaleFactor *= getRandomVec2
			(
				{ 0.98f, 0.98f },
				{ 1.02f, 1.02f }
			);
		}
	}

	void Demo06_Scene::updatePps()
	{
		Shader* postProcessorShader = PostProcessor::getShader();
		PK_ASSERT_QUICK(postProcessorShader != nullptr);
		static float kernel[9];
		const int ppsIndex = m_guiWindow->getPpsIndex();
		switch (ppsIndex)
		{
			case 0: postProcessorShader->setUniform1fv("kernel", 9, kernelIdentity); return;
			case 1: getKernelSharpen(kernel, t); break;
			case 2: getKernelBlur(kernel, t); break;
			case 3: getKernelEdgeDetection(kernel, t); break;
			case 4: getKernelEmboss(kernel, t); break;
			default: PK_LOG_ERROR("Invalid post-processing shader selected in GUI.", "Demo06");
		}
		postProcessorShader->setUniform1fv("kernel", 9, kernel);
	}

	bool Demo06_Scene::onKeyPressed(const Pekan::KeyPressedEvent& event)
	{
		if (event.getKeyCode() == KeyCode::KEY_C)
		{
			m_currentCameraIdx = (m_currentCameraIdx + 1) % 2;
			const bool isFirstCamera = (m_currentCameraIdx == 0);

			CameraComponent2D& cameraComponentFirst = m_registry.get<CameraComponent2D>(m_cameraFirst);
			cameraComponentFirst.isPrimary = isFirstCamera;
			cameraComponentFirst.isControllable = isFirstCamera;

			CameraComponent2D& cameraComponentSecond = m_registry.get<CameraComponent2D>(m_cameraSecond);
			cameraComponentSecond.isPrimary = !isFirstCamera;
			cameraComponentSecond.isControllable = !isFirstCamera;

			return true;
		}
		return false;
	}

} // namespace Demo