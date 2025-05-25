#include "Line.h"

#include "Utils/PekanUtils.h"

#define VERTEX_SHADER_FILEPATH PEKAN_RENDERER_ROOT_DIR "/assets/shaders/VertexShader_2D.glsl"
#define FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER_ROOT_DIR "/assets/shaders/FragmentShader_SolidColor.glsl"

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
			Utils::readFileToString(VERTEX_SHADER_FILEPATH).c_str(),
			Utils::readFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
		);

		setColor(DEFAULT_COLOR);
	}

	void Line::destroy()
	{
		PK_ASSERT(m_renderObject.isValid(), "Trying to destroy a Line that is not yet created.", "Pekan");
		m_renderObject.destroy();
	}

	void Line::render()
	{
		PK_ASSERT(m_renderObject.isValid(), "Trying to render a Line that is not yet created.", "Pekan");
		m_renderObject.bind();
		PekanRenderer::draw(2, DrawMode::Lines);
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