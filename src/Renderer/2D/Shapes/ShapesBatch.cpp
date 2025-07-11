#include "ShapesBatch.h"

#include "PekanLogger.h"
#include "Utils/FileUtils.h"

using namespace Pekan::Graphics;

#define VERTEX_SHADER_FILEPATH PEKAN_RENDERER_ROOT_DIR "/shaders/2D_BatchRendering_VertexShader.glsl"
#define FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER_ROOT_DIR "/shaders/SolidColor_BatchRendering_FragmentShader.glsl"

namespace Pekan
{
namespace Renderer
{

	void ShapesBatch::create(BufferDataUsage bufferDataUsage)
	{
		PK_ASSERT(!m_isValid, "Trying to create a ShapesBatch instance that is already created.", "Pekan");

		// Create underlying render object with empty vertex data
		m_renderObject.create
		(
			nullptr,
			0,
			{ { ShaderDataType::Float2, "position" }, { ShaderDataType::Float4, "color" } },
			bufferDataUsage,
			FileUtils::readFileToString(VERTEX_SHADER_FILEPATH).c_str(),
			FileUtils::readFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
		);
		// and empty index data
		// (we need to explicitly set empty index data because we are also setting data usage)
		m_renderObject.setIndexData(nullptr, 0, bufferDataUsage);

		// Set shader's view projection matrix uniform to a default view projection matrix
		static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
		m_renderObject.getShader().setUniformMatrix4fv("u_viewProjectionMatrix", defaultViewProjectionMatrix);

		m_isValid = true;
	}

	void ShapesBatch::destroy()
	{
		PK_ASSERT(m_isValid, "Trying to destroy a ShapesBatch instance that is not yet created.", "Pekan");

		m_renderObject.destroy();
		m_vertices.clear();
		m_indices.clear();

		m_isValid = false;
	}

	void ShapesBatch::addShape(const Shape& shape)
	{
		PK_ASSERT(m_isValid, "Trying to add a shape to a ShapesBatch that is not yet created.", "Pekan");

		const unsigned oldVerticesSize = unsigned(m_vertices.size());
		const size_t oldIndicesSize = m_indices.size();

		// Get shape's vertices
		const ShapeVertex* vertices = shape.getVertices();
		const int verticesCount = shape.getVerticesCount();
		// Add shape's vertices to the batch
		m_vertices.insert(m_vertices.end(), vertices, vertices + verticesCount);

		// Get shape's indices
		const unsigned* zeroBasedIndices = shape.getIndices();
		const int indicesCount = shape.getIndicesCount();
		// Add shape's indices to the batch,
		// first making each index relative to where shape's vertices begin in the vertices list.
		// Shape's indices are "zero based" meaning they are relative to 0, starting at 0.
		// That's because internally shape's vertices also start at 0,
		// but now that we added them to the vertices list they no longer start at 0.
		// They start at however many vertices there were in the list before adding them,
		// so that's why we add oldVerticesSize to each index.
		m_indices.resize(m_indices.size() + indicesCount);
		for (size_t i = oldIndicesSize; i < m_indices.size(); i++)
		{
			m_indices[i] = zeroBasedIndices[i - oldIndicesSize] + oldVerticesSize;
		}
	}

	void ShapesBatch::render(const Camera2DPtr& camera)
	{
		PK_ASSERT(m_isValid, "Trying to render a ShapesBatch that is not yet created.", "Pekan");

		// If camera is null, render without a camera
		if (camera == nullptr)
		{
			PK_ASSERT(false, "Trying to render a ShapesBatch with a null camera.", "Pekan");
			render();
			return;
		}

		// Set underlying render object's vertex data and index data
		// to the data of our vertices list and indices list
		m_renderObject.setVertexData(m_vertices.data(), m_vertices.size() * sizeof(ShapeVertex));
		m_renderObject.setIndexData(m_indices.data(), m_indices.size() * sizeof(unsigned));

		// Set shader's view projection matrix uniform to camera's transform
		const glm::mat4& viewProjectionMatrix = camera->getViewProjectionMatrix();
		m_renderObject.getShader().setUniformMatrix4fv("u_viewProjectionMatrix", viewProjectionMatrix);

		// Draw all triangles making up all shapes from the batch
		RenderCommands::drawIndexed(m_indices.size());
	}

	void ShapesBatch::render()
	{
		PK_ASSERT(m_isValid, "Trying to render a ShapesBatch that is not yet created.", "Pekan");

		// Set underlying render object's vertex data and index data
		// to the data of our vertices list and indices list
		m_renderObject.setVertexData(m_vertices.data(), m_vertices.size() * sizeof(ShapeVertex));
		m_renderObject.setIndexData(m_indices.data(), m_indices.size() * sizeof(unsigned));

		// Set shader's view projection matrix uniform to a default view projection matrix
		static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
		m_renderObject.getShader().setUniformMatrix4fv("u_viewProjectionMatrix", defaultViewProjectionMatrix);

		// Draw all triangles making up all shapes from the batch
		RenderCommands::drawIndexed(m_indices.size());
	}

	void ShapesBatch::clear()
	{
		PK_ASSERT(m_isValid, "Trying to clear a ShapesBatch that is not yet created.", "Pekan");

		m_vertices.clear();
		m_indices.clear();
	}

} // namespace Renderer
} // namespace Pekan