#include "ShapesBatch.h"

#include "PekanLogger.h"
#include "Utils/FileUtils.h"

using namespace Pekan::Graphics;

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
	#define VERTEX_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shapes/Shaders/2D_Shapes_BatchRendering_1DTexture_VertexShader.glsl"
	#define FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shapes/Shaders/2D_Shapes_BatchRendering_SolidColor_1DTexture_FragmentShader.glsl"
#else
	#define VERTEX_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shapes/Shaders/2D_Shapes_BatchRendering_VertexShader.glsl"
	#define FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shapes/Shaders/2D_Shapes_BatchRendering_SolidColor_FragmentShader.glsl"
#endif

namespace Pekan
{
namespace Renderer2D
{

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
	// A batch's capacity for colors will be this fraction of the maximum texture size supported on current hardware.
	// For example, if this fraction is 80% and hardware's maximum texture size is 1024,
	// then colors capacity will be 0.8 * 1024 = 819.
	static constexpr float CAPACITY_COLORS_FRACTION_OF_MAX_TEXTURE_SIZE = 0.95f;
	// Expected average number of vertices per shape.
	// Used for estimation of size of pre-allocated memory.
	static constexpr int EXPECTED_VERTICES_PER_SHAPE = 24;
	// Expected average number of indices per shape
	// Used for estimation of size of pre-allocated memory.
	static constexpr int EXPECTED_INDICES_PER_SHAPE = 66;
#else
	// A batch's capacity for vertices
	static constexpr int DEFAULT_CAPACITY_VERTICES = 100000;
	// A batch's capacity for indices
	static constexpr int DEFAULT_CAPACITY_INDICES = 150000;
#endif

	// Sets "uViewProjectionMatrix" uniform inside a given shader using a given camera.
	static void setViewProjectionMatrixUniform(Shader& shader, const Camera2D_ConstPtr& camera)
	{
		if (camera != nullptr)
		{
			// Set shader's view projection matrix uniform to camera's view projection matrix
			const glm::mat4& viewProjectionMatrix = camera->getViewProjectionMatrix();
			shader.setUniformMatrix4fv("uViewProjectionMatrix", viewProjectionMatrix);
		}
		else
		{
			// Set shader's view projection matrix uniform to a default view projection matrix
			static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
			shader.setUniformMatrix4fv("uViewProjectionMatrix", defaultViewProjectionMatrix);
		}
	}

	void ShapesBatch::create(BufferDataUsage bufferDataUsage)
	{
		PK_ASSERT(!m_isValid, "Trying to create a ShapesBatch instance that is already created.", "Pekan");

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		// Set batch's capacity for colors to be some fraction of the maximum texture size supported on current hardware
		m_capacityColors = int(float(RenderState::getMaxTextureSize()) * CAPACITY_COLORS_FRACTION_OF_MAX_TEXTURE_SIZE);
		// Each shape has exactly 1 color, so colors capacity is also shapes capacity.
		// Once we know shapes capacity of our batch how can we set good vertices capacity and indices capacity?
		// We don't know the exact number of vertices/indices of a shape because each shape type has a different number of vertices/indices.
		// If we pick a number too small for vertices capacity, for example, then the batch will be limited by this arbitrary number that we picked,
		// instead of being limited by the colors capacity which is what actually matters.
		// If we pick a number too big for vertices capacity, then the batch will be correctly limited by the colors capacity,
		// but we will have wasted a lot of memory for vertices that we will not use - we might use 20% of the memory that we allocated,
		// because when we reached that 20% the colors capacity ran out and we had to stop.
		// ...
		// The solution we do here is to use some expected average for the number of vertices/indices of a shape,
		// so that it can behave well on average.
		m_capacityVertices = EXPECTED_VERTICES_PER_SHAPE * m_capacityColors;
		m_capacityIndices = EXPECTED_INDICES_PER_SHAPE * m_capacityColors;
#else
		m_capacityVertices = DEFAULT_CAPACITY_VERTICES;
		m_capacityIndices = DEFAULT_CAPACITY_INDICES;
#endif

		// Create underlying render object, pre-allocating memory for vertex data
		m_renderObject.create
		(
			nullptr,
			m_capacityVertices * sizeof(ShapeVertex),
#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
			{ { ShaderDataType::Float2, "position" }, { ShaderDataType::Float, "shapeIndex" } },
#else
			{ { ShaderDataType::Float2, "position" }, { ShaderDataType::Float4, "color" } },
#endif
			bufferDataUsage,
			FileUtils::readFileToString(VERTEX_SHADER_FILEPATH).c_str(),
			FileUtils::readFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
		);
		// Pre-allocate memory for index data
		m_renderObject.setIndexData(nullptr, m_capacityIndices * sizeof(unsigned), bufferDataUsage);

		// Set shader's view projection matrix uniform to a default view projection matrix
		static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
		m_renderObject.getShader().setUniformMatrix4fv("uViewProjectionMatrix", defaultViewProjectionMatrix);

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		// Create underlying texture object with empty data
		m_texture.create();
		// IMPORTANT: Set wrap mode to ClampToEdge, because for shader's calculations
		//            we need 0.0 to mean the first color and 1.0 to mean the last color.
		//            If we leave the default wrap mode (ClampToBorder) than 1.0 will be the border color instead.
		m_texture.setWrapMode(TextureWrapMode::ClampToEdge);

		m_shapesCount = 0;
#endif

		m_isValid = true;
	}

	void ShapesBatch::destroy()
	{
		PK_ASSERT(m_isValid, "Trying to destroy a ShapesBatch instance that is not yet created.", "Pekan");

		m_renderObject.destroy();
#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		m_texture.destroy();
#endif

		clear();

		m_isValid = false;
	}

	bool ShapesBatch::addShape(const Shape& shape)
	{
		PK_ASSERT(m_isValid, "Trying to add a shape to a ShapesBatch that is not yet created.", "Pekan");

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		// Set shape's index,
		// letting it know what its index is inside of the batch,
		// so that it can set the "shapeIndex" attribute to its vertices.
		shape.setShapeIndex(float(m_shapesCount));
#endif

		// Get shape's vertices
		const ShapeVertex* vertices = shape.getVertices();
		const int verticesCount = shape.getVerticesCount();
		// Get shape's indices
		const unsigned* zeroBasedIndices = shape.getIndices();
		const int indicesCount = shape.getIndicesCount();
#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		// Get shape's color
		glm::vec4 color = shape.getColor();
#endif

		// If adding this shape would overflow the batch, don't add it
		if (wouldOverflow(verticesCount, indicesCount))
		{
			return false;
		}

		const unsigned oldVerticesSize = unsigned(m_vertices.size());
		const size_t oldIndicesSize = m_indices.size();

		// Add shape's vertices to the batch
		m_vertices.insert(m_vertices.end(), vertices, vertices + verticesCount);

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

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		// Add shape's color to the batch
		m_colors.push_back(color);

		m_shapesCount++;
#endif

		return true;
	}

	void ShapesBatch::render(const Camera2D_ConstPtr& camera)
	{
		PK_ASSERT(m_isValid, "Trying to render a ShapesBatch that is not yet created.", "Pekan");

		// Set underlying render object's vertex data and index data
		// to the data of our vertices list and indices list
		m_renderObject.setVertexSubData(m_vertices.data(), 0, m_vertices.size() * sizeof(ShapeVertex));
		m_renderObject.setIndexSubData(m_indices.data(), 0, m_indices.size() * sizeof(unsigned));

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		// Set underlying texture's colors to the list of shapes' colors
		m_texture.setColors(m_colors);
		// Bind texture to slot 0
		m_texture.bind(0);
#endif

		Shader& shader = m_renderObject.getShader();
		setViewProjectionMatrixUniform(shader, camera);
#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		// Set shader's "uShapesColorsTexture" uniform to be 0, matching the slot where texture is bound.
		shader.setUniform1i("uShapesColorsTexture", 0);
		// Set shader's "uShapesCount" uniform to be the number of shapes in the batch.
		shader.setUniform1i("uShapesCount", m_shapesCount);
#endif

		// Draw all triangles making up all shapes from the batch
		RenderCommands::drawIndexed(m_indices.size());
	}

	void ShapesBatch::clear()
	{
		PK_ASSERT(m_isValid, "Trying to clear a ShapesBatch that is not yet created.", "Pekan");

		m_vertices.clear();
		m_indices.clear();
#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		m_colors.clear();

		m_shapesCount = 0;
#endif
	}

	bool ShapesBatch::wouldOverflow(int verticesCount, int indicesCount) const
	{
		return
		(
			m_vertices.size() + verticesCount > m_capacityVertices
			|| m_indices.size() + indicesCount > m_capacityIndices
#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
			|| m_colors.size() + 1 > m_capacityColors
#endif
		);
	}

} // namespace Renderer2D
} // namespace Pekan