#include "Line.h"

#include "PekanLogger.h"
#include "Utils/FileUtils.h"
#include "Renderer2DSystem.h"

#define VERTEX_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shaders/2D_Line_VertexShader.glsl"
#define FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shaders/2D_Line_SolidColor_FragmentShader.glsl"

static constexpr long long VERTEX_DATA_SIZE = 4 * sizeof(float);
static constexpr glm::vec4 DEFAULT_COLOR = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

using namespace Pekan::Graphics;

namespace Pekan
{
namespace Renderer2D
{

	void Line::create(glm::vec2 pointA, glm::vec2 pointB)
	{
		m_vertices[0] = pointA;
		m_vertices[1] = pointB;

		m_renderObject.create
		(
			m_vertices, VERTEX_DATA_SIZE,
			{ { ShaderDataType::Float2, "position" } },
			BufferDataUsage::DynamicDraw,
			FileUtils::readTextFileToString(VERTEX_SHADER_FILEPATH).c_str(),
			FileUtils::readTextFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
		);

		// Set shader's view projection matrix uniform to an identity matrix
		static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
		m_renderObject.getShader().setUniformMatrix4fv("uViewProjectionMatrix", defaultViewProjectionMatrix);

		setColor(DEFAULT_COLOR);
	}

	void Line::destroy()
	{
		PK_ASSERT(m_renderObject.isValid(), "Trying to destroy a Line that is not yet created.", "Pekan");
		m_renderObject.destroy();
	}

	void Line::update()
	{
		// Get current camera
		Camera2D_ConstPtr camera = Renderer2DSystem::getCamera();
		if (camera != nullptr)
		{
			// Set shader's view projection matrix uniform to camera's transform
			const glm::mat4& viewProjectionMatrix = camera->getViewProjectionMatrix();
			m_renderObject.getShader().setUniformMatrix4fv("uViewProjectionMatrix", viewProjectionMatrix);
		}
		else
		{
			// Set shader's view projection matrix uniform to a default view projection matrix
			static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
			m_renderObject.getShader().setUniformMatrix4fv("uViewProjectionMatrix", defaultViewProjectionMatrix);
		}
	}

	void Line::render() const
	{
		PK_ASSERT(m_renderObject.isValid(), "Trying to render a Line that is not yet created.", "Pekan");
		m_renderObject.render(DrawMode::Lines);
	}

	void Line::setPointA(glm::vec2 pointA)
	{
		m_vertices[0] = pointA;
		m_renderObject.setVertexData(m_vertices, VERTEX_DATA_SIZE);
	}

	void Line::setPointB(glm::vec2 pointB)
	{
		m_vertices[1] = pointB;
		m_renderObject.setVertexData(m_vertices, VERTEX_DATA_SIZE);
	}

	void Line::setColor(glm::vec4 color)
	{
		PK_ASSERT(m_renderObject.isValid(), "Trying to set color of a Line that is not yet created.", "Pekan");
		m_color = color;
		m_renderObject.getShader().setUniform4fv("uColor", color);
	}

} // namespace Renderer2D
} // namespace Renderer2D