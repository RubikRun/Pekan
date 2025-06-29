#include "Line.h"

#include "Utils/FileUtils.h"
#include "RenderCommands.h"

#define VERTEX_SHADER_FILEPATH PEKAN_RENDERER_ROOT_DIR "/shaders/2D_VertexShader.glsl"
#define FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER_ROOT_DIR "/shaders/SolidColor_FragmentShader.glsl"

static constexpr long long VERTEX_DATA_SIZE = 4 * sizeof(float);

static constexpr glm::vec4 DEFAULT_COLOR = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

namespace Pekan
{
namespace Renderer
{

	void Line::create(glm::vec2 pointA, glm::vec2 pointB, bool dynamic)
	{
		const BufferDataUsage vertexDataUsage = (dynamic ? BufferDataUsage::DynamicDraw : BufferDataUsage::StaticDraw);

		m_vertices[0] = pointA;
		m_vertices[1] = pointB;

		m_renderObject.create
		(
			m_vertices, VERTEX_DATA_SIZE,
			{ { ShaderDataType::Float2, "position" } }, vertexDataUsage,
			FileUtils::readFileToString(VERTEX_SHADER_FILEPATH).c_str(),
			FileUtils::readFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
		);

		// Set shader's view projection matrix uniform to an identity matrix
		static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
		m_renderObject.getShader().setUniformMatrix4fv("u_viewProjectionMatrix", defaultViewProjectionMatrix);

		setColor(DEFAULT_COLOR);
	}

	void Line::destroy()
	{
		PK_ASSERT(m_renderObject.isValid(), "Trying to destroy a Line that is not yet created.", "Pekan");
		m_renderObject.destroy();
	}

	void Line::render() const
	{
		PK_ASSERT(m_renderObject.isValid(), "Trying to render a Line that is not yet created.", "Pekan");
		m_renderObject.bind();
		RenderCommands::draw(2, DrawMode::Lines);
	}

	void Line::render(const Camera2D& camera)
	{
		PK_ASSERT(m_renderObject.isValid(), "Trying to render a Line that is not yet created.", "Pekan");
		m_renderObject.bind();

		// Set shader's view projection matrix uniform
		const glm::mat4& viewProjectionMatrix = camera.getViewProjectionMatrix();
		m_renderObject.getShader().setUniformMatrix4fv("u_viewProjectionMatrix", viewProjectionMatrix);

		RenderCommands::draw(2, DrawMode::Lines);
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

} // namespace Renderer
} // namespace Renderer