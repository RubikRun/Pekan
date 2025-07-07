#include "Renderer2D.h"

#include "Utils/FileUtils.h"
#include "SubsystemManager.h"
#include "Graphics.h"

#define VERTEX_SHADER_FILEPATH PEKAN_RENDERER_ROOT_DIR "/shaders/2D_BatchRendering_VertexShader.glsl"
#define FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER_ROOT_DIR "/shaders/SolidColor_BatchRendering_FragmentShader.glsl"

using namespace Pekan::Graphics;

namespace Pekan
{
namespace Renderer
{

	static Renderer2D g_renderer2D;
	
	void Renderer2D::registerSubsystem()
	{
		SubsystemManager::registerSubsystem(&g_renderer2D);
	}

	RenderObject Renderer2D::s_batch;
	Renderer2D::CameraWeakPtr Renderer2D::s_camera;
	std::vector<ShapeVertex> Renderer2D::s_vertices;
	std::vector<unsigned> Renderer2D::s_indices;

	void Renderer2D::beginFrame()
	{
		s_vertices.clear();
		s_indices.clear();
	}

	void Renderer2D::endFrame()
	{
		s_batch.setVertexData(s_vertices.data(), s_vertices.size() * sizeof(ShapeVertex));
		s_batch.setIndexData(s_indices.data(), s_indices.size() * sizeof(unsigned));

		CameraSharedPtr camera = s_camera.lock();
		// If camera still exists use its transform to set shader's view projection matrix uniform
		if (camera != nullptr)
		{
			const glm::mat4& viewProjectionMatrix = camera->getViewProjectionMatrix();
			s_batch.getShader().setUniformMatrix4fv("u_viewProjectionMatrix", viewProjectionMatrix);
		}
		// Otherwise, use a default view projection matrix
		else
		{
			static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
			s_batch.getShader().setUniformMatrix4fv("u_viewProjectionMatrix", defaultViewProjectionMatrix);
		}

		RenderCommands::drawIndexed(s_indices.size());
	}

	void Renderer2D::init()
	{
		// Create shapes batch with empty vertex data
		s_batch.create
		(
			s_vertices.data(),
			0,
			{ { ShaderDataType::Float2, "position" }, { ShaderDataType::Float4, "color" } },
			BufferDataUsage::DynamicDraw,
			FileUtils::readFileToString(VERTEX_SHADER_FILEPATH).c_str(),
			FileUtils::readFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
		);
		// Set empty index data to shapes batch
		s_batch.setIndexData(s_indices.data(), 0, BufferDataUsage::DynamicDraw);

		// Set shader's view projection matrix uniform to a default view projection matrix
		static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
		s_batch.getShader().setUniformMatrix4fv("u_viewProjectionMatrix", defaultViewProjectionMatrix);

		m_isInitialized = true;
	}

	void Renderer2D::exit()
	{
		s_batch.destroy();
		s_vertices.clear();
		s_indices.clear();

		m_isInitialized = false;
	}

	ISubsystem* Renderer2D::getParent()
	{
		return Graphics::Graphics::getInstance();
	}

	void Renderer2D::render(const Shape& shape)
	{
		const unsigned oldVerticesSize = unsigned(s_vertices.size());
		const size_t oldIndicesSize = s_indices.size();

		// Get shape's vertices
		const ShapeVertex* vertices = shape.getVertices();
		const int verticesCount = shape.getVerticesCount();
		// Add shape's vertices to the batch
		s_vertices.insert(s_vertices.end(), vertices, vertices + verticesCount);

		// Get shape's indices
		const unsigned* localIndices = shape.getIndices();
		const int indicesCount = shape.getIndicesCount();
		// Add shape's indices to the batch,
		// adding the old size of vertices list (before adding the new vertices) to each index,
		// so that they correctly reference vertices from the batch.
		s_indices.resize(s_indices.size() + indicesCount);
		for (size_t i = oldIndicesSize; i < s_indices.size(); i++)
		{
			s_indices[i] = localIndices[i - oldIndicesSize] + oldVerticesSize;
		}
	}

} // namespace Renderer
} // namespace Pekan