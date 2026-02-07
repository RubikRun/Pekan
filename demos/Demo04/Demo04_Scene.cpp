#include "Demo04_Scene.h"
#include "PekanLogger.h"
#include "Utils/FileUtils.h"

#include "TransformComponent2D.h"
#include "TriangleGeometryComponent.h"
#include "RectangleGeometryComponent.h"
#include "CircleGeometryComponent.h"
#include "PolygonGeometryComponent.h"
#include "SolidColorMaterialComponent.h"

#include <glm/gtc/constants.hpp>
constexpr float PI = glm::pi<float>();

constexpr const char* const VERTEX_SHADER_FILEPATHS[] =
{
	"resources/04_00_vertex_shader.glsl",
	"resources/04_01_vertex_shader.glsl",
	"resources/04_02_vertex_shader.glsl",
	"resources/04_03_vertex_shader.glsl"
};

constexpr const char* const FRAGMENT_SHADER_FILEPATHS[] =
{
	"resources/04_00_fragment_shader.glsl",
	"resources/04_01_fragment_shader.glsl",
	"resources/04_02_fragment_shader.glsl",
	"resources/04_03_fragment_shader.glsl"
};

constexpr const char* const IMAGE0_FILEPATH = "resources/tmnt.png";
constexpr const char* const IMAGE1_FILEPATH = "resources/powerpuff.png";

constexpr int POLYGON1_VERTICES_COUNT = 7;
constexpr float POLYGON1_RADIUS = 0.2f;
const std::vector<glm::vec2> POLYGON2_VERTICES =
{
	glm::vec2( 0.20040983606557375, 0.14385245901639346 ),
	glm::vec2( 0.1081967213114754, 0.11987704918032788 ),
	glm::vec2( 0.03688524590163935, 0.1653688524590164 ),
	glm::vec2( 0.06147540983606554, 0.11741803278688524 ),
	glm::vec2( -0.0264344262295082, 0.1112704918032787 ),
	glm::vec2( 0.0049180327868852515, 0.09651639344262294 ),
	glm::vec2( -0.023360655737704926, 0.06516393442622949 ),
	glm::vec2( -0.0540983606557377, 0.1014344262295082 ),
	glm::vec2( -0.04795081967213115, 0.14323770491803275 ),
	glm::vec2( 0.04057377049180331, 0.1413934426229508 ),
	glm::vec2( 0.03995901639344259, 0.14692622950819673 ),
	glm::vec2( -0.04795081967213115, 0.21454918032786885 ),
	glm::vec2( -0.10573770491803276, 0.08114754098360655 ),
	glm::vec2( -0.1875, 0.1973360655737705 ),
	glm::vec2( -0.21577868852459015, 0.003073770491803274 ),
	glm::vec2( -0.2723360655737705, -0.07438524590163935 ),
	glm::vec2( -0.21024590163934423, -0.16045081967213115 ),
	glm::vec2( -0.10020491803278687, -0.1930327868852459 ),
	glm::vec2( -0.0110655737704918, -0.0737704918032787 ),
	glm::vec2( -0.014139344262295073, -0.07069672131147539 ),
	glm::vec2( -0.10020491803278687, -0.15799180327868848 ),
	glm::vec2( -0.15061475409836064, -0.1444672131147541 ),
	glm::vec2( -0.09098360655737704, -0.027663934426229497 ),
	glm::vec2( -0.1069672131147541, -0.12540983606557374 ),
	glm::vec2( 0.030737704918032804, 0.05655737704918032 ),
	glm::vec2( 0.0805327868852459, -0.08545081967213115 ),
	glm::vec2( 0.07315573770491805, -0.0055327868852459 ),
	glm::vec2( 0.21209016393442626, -0.094672131147541 ),
	glm::vec2( 0.22622950819672127, 0.08790983606557377 ),
	glm::vec2( 0.1641393442622951, 0.007377049180327877 ),
	glm::vec2( 0.15799180327868848, 0.09713114754098359 ),
	glm::vec2( 0.1875, 0.06823770491803277 )
};

using namespace Pekan;
using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;

namespace Demo
{

	bool Demo04_Scene::_init()
	{
		if (m_guiWindow == nullptr)
		{
			PK_LOG_ERROR("Cannot initialize Demo04_Scene without a GUI window attached.", "Demo04");
			return false;
		}

		RenderState::enableMultisampleAntiAliasing();
		// Enable and configure blending
		RenderState::enableBlending();
		RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

		// Set up vertex data and configure vertex attributes
		static constexpr float vertices[] =
		{
			-0.8f, -0.8f, 0.0f, 0.0f, // bottom left
			 0.8f, -0.8f, 1.0f, 0.0f, // bottom right
			 0.8f,  0.8f, 1.0f, 1.0f, // top right
			-0.8f,  0.8f, 0.0f, 1.0f  // top left
		};
		static constexpr unsigned indices[] =
		{
			0, 1, 2,
			0, 2, 3
		};

		m_shaderIdx = m_guiWindow->getShaderIdx();

		m_drawObject.create
		(
			vertices, sizeof(vertices),
			{ { ShaderDataType::Float2, "position" }, { ShaderDataType::Float2, "texCoord" } },
			BufferDataUsage::StaticDraw,
			Pekan::FileUtils::readTextFileToString(VERTEX_SHADER_FILEPATHS[m_shaderIdx]).c_str(),
			Pekan::FileUtils::readTextFileToString(FRAGMENT_SHADER_FILEPATHS[m_shaderIdx]).c_str()
		);
		m_drawObject.setIndexData(indices, sizeof(indices), BufferDataUsage::StaticDraw);

		// Load the two images
		m_image0.load(IMAGE0_FILEPATH);
		m_image1.load(IMAGE1_FILEPATH);
		// Set two images as textures to the draw object
		m_drawObject.setTextureImage(m_image0, "uTex0", 0);
		m_drawObject.setTextureImage(m_image1, "uTex1", 1);

		m_triangle = createEntity();
		m_registry.emplace<TransformComponent2D>(m_triangle);
		m_registry.emplace<TriangleGeometryComponent>(m_triangle);
		m_registry.emplace<SolidColorMaterialComponent>(m_triangle);

		m_rectangleInitialWidth = 0.2f;
		m_rectangleInitialHeight = 0.4f;
		m_rectangleInitialPosition = { -0.7f, -0.7f };
		m_rectangle = createEntity();
		m_registry.emplace<RectangleGeometryComponent>(m_rectangle, m_rectangleInitialWidth, m_rectangleInitialHeight);
		m_registry.emplace<TransformComponent2D>(m_rectangle, m_rectangleInitialPosition);
		m_registry.emplace<SolidColorMaterialComponent>(m_rectangle);

		m_circleInitialRadius = 0.3f;
		m_circleInitialPosition = { 0.0f, 0.0f };
		m_circle = createEntity();
		m_registry.emplace<TransformComponent2D>(m_circle, m_circleInitialPosition);
		m_registry.emplace<CircleGeometryComponent>(m_circle, m_circleInitialRadius);
		m_registry.emplace<SolidColorMaterialComponent>(m_circle);

		m_polygon1InitialPosition = { 0.7f, -0.7f };
		m_polygon1 = createEntity();
		m_registry.emplace<TransformComponent2D>(m_polygon1, m_polygon1InitialPosition);
		m_registry.emplace<PolygonGeometryComponent>(m_polygon1);
		m_registry.emplace<SolidColorMaterialComponent>(m_polygon1);

		m_polygon2InitialPosition = { -0.7f, 0.7f };
		m_polygon2 = createEntity();
		m_registry.emplace<TransformComponent2D>(m_polygon2, m_polygon2InitialPosition);
		m_registry.emplace<PolygonGeometryComponent>(m_polygon2, POLYGON2_VERTICES);
		m_registry.emplace<SolidColorMaterialComponent>(m_polygon2);

		m_enabledFaceCulling = m_guiWindow->getEnabledFaceCulling();
		if (m_enabledFaceCulling)
		{
			RenderState::enableFaceCulling();
		}
		else
		{
			RenderState::disableFaceCulling();
		}

		// Set previous enabled shapes state to opposite of current, to force update on first frame
		m_prevIsEnabledShapes = !m_guiWindow->isEnabledShapes();

		t = 0.0f;

		return true;
	}

	void Demo04_Scene::_exit()
	{
		m_drawObject.destroy();
		destroyEntity(m_polygon2);
		destroyEntity(m_polygon1);
		destroyEntity(m_circle);
		destroyEntity(m_rectangle);
		destroyEntity(m_triangle);
	}

	// Oscillates between 0 and 1 in a sine wave, as x grows
	static float osc(float x)
	{
		return (cos(x) + 1.0f) / 2.0f;
	}
	static float osc(float x, float a, float b)
	{
		return a + (b - a) * osc(x);
	}

	void Demo04_Scene::update(double dt)
	{
		PK_ASSERT_QUICK(m_guiWindow != nullptr);
		// Set background color from GUI window
		const glm::vec3 backgroundColor = m_guiWindow->getBackgroundColor();
		RenderState::setBackgroundColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);

		Shader& texRectShader = m_drawObject.getShader();
		texRectShader.bind();

		if (m_guiWindow != nullptr)
		{
			// If selected shader in GUI has changed
			const int guiShaderIdx = m_guiWindow->getShaderIdx();
			if (m_shaderIdx != guiShaderIdx)
			{
				m_shaderIdx = guiShaderIdx;
				// then set new shader's source code
				m_drawObject.setShaderSource
				(
					Pekan::FileUtils::readTextFileToString(VERTEX_SHADER_FILEPATHS[m_shaderIdx]).c_str(),
					Pekan::FileUtils::readTextFileToString(FRAGMENT_SHADER_FILEPATHS[m_shaderIdx]).c_str()
				);
				// We need to set textures again after changing the shader
				m_drawObject.setTextureImage(m_image0, "uTex0", 0);
				m_drawObject.setTextureImage(m_image1, "uTex1", 1);
			}

			// If enabled/disabled state of face culling has changed in GUI
			if (m_enabledFaceCulling != m_guiWindow->getEnabledFaceCulling())
			{
				m_enabledFaceCulling = !m_enabledFaceCulling;
				if (m_enabledFaceCulling)
				{
					RenderState::enableFaceCulling();
				}
				else
				{
					RenderState::disableFaceCulling();
				}
			}

			// Get position from GUI
			const glm::vec2 position = m_guiWindow->getPosition();
			// Set "uPosition" uniform inside of the shader
			texRectShader.setUniform2f("uPosition", position);
		}

		texRectShader.setUniform1f("uTime", t);

		TransformComponent2D& triangleTransform = m_registry.get<TransformComponent2D>(m_triangle);
		TriangleGeometryComponent& triangleGeometry = m_registry.get<TriangleGeometryComponent>(m_triangle);
		SolidColorMaterialComponent& triangleMaterial = m_registry.get<SolidColorMaterialComponent>(m_triangle);
		triangleTransform.position = glm::vec2(0.8f, 0.8f) + glm::vec2(sin(t) * 0.1f, sin(t / 4.0f) * 0.05f);
		triangleMaterial.color = { osc(t), osc(t / 2.0f + 2.0f), osc(t / 3.0f), osc(t / 3.0f, 0.3f, 1.0f) };

		if (m_guiWindow != nullptr && m_guiWindow->getReverseTriangleOrientation())
		{
			triangleGeometry.pointA = glm::vec2(0.1f, 0.1f) + glm::vec2(0.0f, sin(t / 5.0f) * 0.03f);
			triangleGeometry.pointB = glm::vec2(0.1f, -0.1f) + glm::vec2(cos(t * 2.0f) * sin(t) * 0.05f, sin(t * 0.83f) * 0.1f);
			triangleGeometry.pointC = glm::vec2(-0.1f, -0.1f) + glm::vec2(cos(t) * 0.1f, sin(t) * 0.1f);
		}
		else
		{
			triangleGeometry.pointA = glm::vec2(-0.1f, -0.1f) + glm::vec2(cos(t) * 0.1f, sin(t) * 0.1f);
			triangleGeometry.pointB = glm::vec2(0.1f, -0.1f) + glm::vec2(cos(t * 2.0f) * sin(t) * 0.05f, sin(t * 0.83f) * 0.1f);
			triangleGeometry.pointC = glm::vec2(0.1f, 0.1f) + glm::vec2(0.0f, sin(t / 5.0f) * 0.03f);
		}

		TransformComponent2D& rectangleTransform = m_registry.get<TransformComponent2D>(m_rectangle);
		RectangleGeometryComponent& rectangleGeometry = m_registry.get<RectangleGeometryComponent>(m_rectangle);
		SolidColorMaterialComponent& rectangleMaterial = m_registry.get<SolidColorMaterialComponent>(m_rectangle);

		rectangleTransform.position = m_rectangleInitialPosition + glm::vec2(sin(t / 2.0f) * 0.12f, sin(t / 5.0f) * 0.04f);
		rectangleMaterial.color = { osc(t / 2.0f + 1.0f), osc(t), osc(t / 3.0f), osc(t / 7.0f, 0.3f, 1.0f) };
		rectangleGeometry.width = osc(t / 2.0f, m_rectangleInitialWidth * 0.5f, m_rectangleInitialWidth * 1.5f);
		rectangleGeometry.height = osc(t / 5.0f, m_rectangleInitialHeight * 0.5f, m_rectangleInitialHeight * 1.5f);

		CircleGeometryComponent& circleGeometry = m_registry.get<CircleGeometryComponent>(m_circle);
		circleGeometry.segmentsCount = int(osc(t / 5.0f, 3.0f, 30.0f));

		updatePolygon1();
		updatePolygon2();

		// Update disabled state of shapes depending on whether they are enabled in GUI
		{
			const bool isEnabledShapes = m_guiWindow->isEnabledShapes();

			if (isEnabledShapes && !m_prevIsEnabledShapes)
			{
				enableEntity(m_triangle);
				enableEntity(m_rectangle);
				enableEntity(m_circle);
				enableEntity(m_polygon1);
				enableEntity(m_polygon2);
			}
			else if (!isEnabledShapes && m_prevIsEnabledShapes)
			{
				disableEntity(m_triangle);
				disableEntity(m_rectangle);
				disableEntity(m_circle);
				disableEntity(m_polygon1);
				disableEntity(m_polygon2);
			}

			m_prevIsEnabledShapes = isEnabledShapes;
		}

		t += float(dt) * 5.0f;
	}

	void Demo04_Scene::_render() const
	{
		// Manually render m_drawObject since it's not part of ECS and will not be rendered automatically.
		m_drawObject.render();
	}

	void Demo04_Scene::updatePolygon1()
	{
		TransformComponent2D& polygon1Transform = m_registry.get<TransformComponent2D>(m_polygon1);
		PolygonGeometryComponent& polygon1Geometry = m_registry.get<PolygonGeometryComponent>(m_polygon1);
		SolidColorMaterialComponent& polygon1Material = m_registry.get<SolidColorMaterialComponent>(m_polygon1);

		const bool reverseOrientation = (m_guiWindow != nullptr && m_guiWindow->getReversePolygonOrientation());
		const float reverseFactor = reverseOrientation ? -1.0f : 1.0f;
		polygon1Geometry.vertexPositions = std::vector<glm::vec2>(POLYGON1_VERTICES_COUNT, glm::vec2(0.0f, 0.0f));
		const float baseArc = reverseFactor * 2.0f * PI / float(POLYGON1_VERTICES_COUNT);
		for (int i = 0; i < POLYGON1_VERTICES_COUNT; i++)
		{
			const int iRev = reverseOrientation ? (POLYGON1_VERTICES_COUNT - i) % POLYGON1_VERTICES_COUNT : i;
			const float arc = baseArc * float(i);
			polygon1Geometry.vertexPositions[i] = glm::vec2
			(
				cos(arc) * POLYGON1_RADIUS + osc(t / float(iRev + 1), 0.0f, float(iRev + 1) / 190.0f),
				sin(arc) * POLYGON1_RADIUS + osc(t / float(POLYGON1_VERTICES_COUNT - iRev), 0.0f, float(POLYGON1_VERTICES_COUNT - iRev) / 190.0f)
			);
		}

		polygon1Transform.position = m_polygon1InitialPosition + glm::vec2(sin(t / 8.0f) * 0.07f, sin(t / 6.0f) * 0.04f);
		polygon1Material.color = { osc(t / 2.0f), osc(t / 5.0f + 2.0f), osc(t / 11.0f), osc(t / 15.0f, 0.6f, 1.0f) };
	}

	void Demo04_Scene::updatePolygon2()
	{
		TransformComponent2D& polygon2Transform = m_registry.get<TransformComponent2D>(m_polygon2);
		SolidColorMaterialComponent& polygon2Material = m_registry.get<SolidColorMaterialComponent>(m_polygon2);

		polygon2Transform.position = m_polygon2InitialPosition + glm::vec2(sin(t / 8.0f) * 0.02f, sin(t / 6.0f) * 0.03f);
		polygon2Material.color = { osc(t / 3.0f), osc(t / 2.0f + 5.0f), osc(t / 8.0f), osc(t / 11.0f, 0.6f, 1.0f) };
	}

} // namespace Demo