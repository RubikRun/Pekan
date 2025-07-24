#include "ShapesBatch.h"

#include "PekanLogger.h"
#include "Utils/FileUtils.h"

using namespace Pekan::Graphics;

#define VERTEX_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/shaders/2D_BatchRendering_VertexShader.glsl"
#define FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/shaders/SolidColor_BatchRendering_FragmentShader.glsl"

namespace Pekan
{
namespace Renderer2D
{

	// Maximum allowed fraction of hardware's maximum texture size.
	// For example, if this fraction is 80% and hardware's maximum texture size is 1024,
	// then we'll allow up to 0.8 * 1024 texels.
	static const float MAX_TEXTURE_SIZE_FRACTION = 0.8f;
	// Maximum allowed number of vertices per batch.
	static constexpr size_t MAX_VERTICES_PER_BATCH = 10000;
	// Maximum allowed number of indices per batch.
	static constexpr size_t MAX_INDICES_PER_BATCH = 15000;

	void ShapesBatch::create(BufferDataUsage bufferDataUsage)
	{
		PK_ASSERT(!m_isValid, "Trying to create a ShapesBatch instance that is already created.", "Pekan");

		// Create underlying render object with empty vertex data
		m_renderObject.create
		(
			nullptr,
			0,
			{ { ShaderDataType::Float2, "position" }, { ShaderDataType::Float, "shapeIndex" } },
			bufferDataUsage,
			FileUtils::readFileToString(VERTEX_SHADER_FILEPATH).c_str(),
			FileUtils::readFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
		);
		// and empty index data
		// (we need to explicitly set empty index data because we are also setting data usage)
		m_renderObject.setIndexData(nullptr, 0, bufferDataUsage);

		// Set shader's view projection matrix uniform to a default view projection matrix
		static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
		m_renderObject.getShader().setUniformMatrix4fv("uViewProjectionMatrix", defaultViewProjectionMatrix);

		// Create underlying texture object with empty data
		m_texture.create();
		// IMPORTANT: Set wrap mode to ClampToEdge, because for shader's calculations
		//            we need 0.0 to mean the first color and 1.0 to mean the last color.
		//            If we leave the default wrap mode (ClampToBorder) than 1.0 will be the border color instead.
		m_texture.setWrapMode(TextureWrapMode::ClampToEdge);

		m_shapesCount = 0;
		m_isValid = true;
	}

	void ShapesBatch::destroy()
	{
		PK_ASSERT(m_isValid, "Trying to destroy a ShapesBatch instance that is not yet created.", "Pekan");

		m_renderObject.destroy();
		m_texture.destroy();
		clear();

		m_shapesCount = 0;
		m_isValid = false;
	}

	bool ShapesBatch::addShape(const Shape& shape)
	{
		PK_ASSERT(m_isValid, "Trying to add a shape to a ShapesBatch that is not yet created.", "Pekan");

		// A flag indicating if more shapes can be added after this one,
		// or the batch needs to be rendered, and a new one started.
		bool canAddMore = true;

		const unsigned oldVerticesSize = unsigned(m_vertices.size());
		const size_t oldIndicesSize = m_indices.size();

		// Set shape's index,
		// letting it know what its index is inside of the batch,
		// so that it can set the "shapeIndex" attribute to its vertices.
		shape.setShapeIndex(float(m_shapesCount));

		// Get shape's vertices
		const ShapeVertex* vertices = shape.getVertices();
		const int verticesCount = shape.getVerticesCount();
		// Add shape's vertices to the batch
		m_vertices.insert(m_vertices.end(), vertices, vertices + verticesCount);
		// If we reach the maximum number of vertices, we can't add more shapes
		if (m_vertices.size() >= MAX_VERTICES_PER_BATCH)
		{
			canAddMore = false;
		}

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
		// If we reach the maximum number of indices, we can't add more shapes
		if (m_indices.size() >= MAX_INDICES_PER_BATCH)
		{
			canAddMore = false;
		}

		// Get shape's color
		glm::vec4 color = shape.getColor();
		// Add shape's color to the batch
		m_colors.push_back(color);

		// Max number of colors per batch that we'll allow on current hardware.
		static size_t maxColors = size_t(float(RenderState::getMaxTextureSize()) * MAX_TEXTURE_SIZE_FRACTION);
		// If we reach the maximum number of colors, we can't add more shapes
		if (int(m_colors.size()) >= maxColors)
		{
			canAddMore = false;
		}

		m_shapesCount++;

		return canAddMore;
	}

	void ShapesBatch::render(const Camera2D_ConstPtr& camera)
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

		// Set underlying texture's colors to the list of shapes' colors
		m_texture.setColors(m_colors);
		// Bind texture to slot 0
		m_texture.bind(0);

		Shader& shader = m_renderObject.getShader();

		// Set shader's "uShapesColorsTexture" uniform to be 0 - the slot where our texture is bound.
		shader.setUniform1i("uShapesColorsTexture", 0);
		// Set shader's "uShapesCount" uniform to be the number of shapes in the batch.
		shader.setUniform1i("uShapesCount", m_shapesCount);
		// Set shader's view projection matrix uniform to camera's transform
		const glm::mat4& viewProjectionMatrix = camera->getViewProjectionMatrix();
		shader.setUniformMatrix4fv("uViewProjectionMatrix", viewProjectionMatrix);

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

		// Set underlying texture's colors to the list of shapes' colors
		m_texture.setColors(m_colors);
		// Bind texture to slot 0
		m_texture.bind(0);

		Shader& shader = m_renderObject.getShader();

		// Set shader's "uShapesColorsTexture" uniform to be 0 - the slot where our texture is bound.
		shader.setUniform1i("uShapesColorsTexture", 0);
		// Set shader's "uShapesCount" uniform to be the number of shapes in the batch.
		shader.setUniform1i("uShapesCount", m_shapesCount);
		// Set shader's view projection matrix uniform to a default view projection matrix
		static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
		shader.setUniformMatrix4fv("uViewProjectionMatrix", defaultViewProjectionMatrix);

		// Draw all triangles making up all shapes from the batch
		RenderCommands::drawIndexed(m_indices.size());
	}

	void ShapesBatch::clear()
	{
		PK_ASSERT(m_isValid, "Trying to clear a ShapesBatch that is not yet created.", "Pekan");

		m_vertices.clear();
		m_indices.clear();
		m_colors.clear();
		m_shapesCount = 0;
	}

} // namespace Renderer2D
} // namespace Pekan