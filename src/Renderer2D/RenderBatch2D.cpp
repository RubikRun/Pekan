#include "RenderBatch2D.h"

#include "PekanLogger.h"
#include "Utils/FileUtils.h"

using namespace Pekan::Graphics;

#define VERTEX_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shaders/2D_Batch_VertexShader.glsl"
#define FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Shaders/2D_Batch_FragmentShader.glsl"

namespace Pekan
{
namespace Renderer2D
{

	// A batch's capacity for colors will be this fraction of the maximum texture size supported on current hardware.
	// For example, if this fraction is 80% and hardware's maximum texture size is 1024,
	// then colors capacity will be 0.8 * 1024 = 819.
	constexpr float CAPACITY_COLORS_FRACTION_OF_MAX_TEXTURE_SIZE = 0.95f;

	// Sets "uTextures" uniform inside a given shader
	// to a list of texture slots { 0, 1, 2, 3, ... } or { 1, 2, 3, 4, ... }
	// (depending on whether we are using a 1D texture which is bound on slot 0)
	// with as many texture slots as is batch's capacity.
	static void setTexturesUniform(Shader& shader, size_t capacityTextures)
	{
		std::vector<int> textures(capacityTextures);
		for (size_t i = 0; i < capacityTextures; i++)
		{
			textures[i] = int(i + 1);
		}

		shader.setUniform1iv("uTextures", textures.size(), textures.data());
	}

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
			static constexpr glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
			shader.setUniformMatrix4fv("uViewProjectionMatrix", defaultViewProjectionMatrix);
		}
	}

	void RenderBatch2D::create()
	{
		PK_ASSERT(!m_isValid, "Trying to create a RenderBatch2D instance that is already created.", "Pekan");

		// Set batch's capacity for textures to be the maximum number of texture slots supported on current hardware minus one.
		// We subtract one because slot 0 will always be occupied by the 1D colors texture.
		m_capacityTextures = RenderState::getMaxTextureSlots() - 1;
		// Set batch's capacity for colors to be some fraction of the maximum texture size supported on current hardware
		m_capacityColors = int(float(RenderState::getMaxTextureSize()) * CAPACITY_COLORS_FRACTION_OF_MAX_TEXTURE_SIZE);

		// Create underlying render object with empty vertex data
		m_renderObject.create
		(
			nullptr,
			0,
			{
				{ ShaderDataType::Float2, "position" },
				{ ShaderDataType::Float2, "textureCoordinates" },
				{ ShaderDataType::Float, "textureIndex" },
				{ ShaderDataType::Float, "shapeIndex" }
			},
			BufferDataUsage::DynamicDraw,
			FileUtils::readTextFileToString(VERTEX_SHADER_FILEPATH).c_str(),
			FileUtils::readTextFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
		);
		// and empty index data
		// (we need to explicitly set empty index data because we are also setting data usage)
		m_renderObject.setIndexData(nullptr, 0, BufferDataUsage::DynamicDraw);

		// Set shader's view projection matrix uniform to a default view projection matrix
		static constexpr glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
		m_renderObject.getShader().setUniformMatrix4fv("uViewProjectionMatrix", defaultViewProjectionMatrix);

		// Create underlying texture object with empty data
		m_colorsTexture.create();
		// IMPORTANT: Set wrap mode to ClampToEdge, because for shader's calculations
		//            we need 0.0 to mean the first color and 1.0 to mean the last color.
		//            If we leave the default wrap mode (ClampToBorder) than 1.0 will be the border color instead.
		m_colorsTexture.setWrapMode(TextureWrapMode::ClampToEdge);

		m_colorsCount = 0;

		m_isValid = true;
	}

	void RenderBatch2D::destroy()
	{
		PK_ASSERT(m_isValid, "Trying to destroy a RenderBatch2D instance that is not yet created.", "Pekan");

		m_renderObject.destroy();
		m_colorsTexture.destroy();

		clear();

		m_isValid = false;
	}

	bool RenderBatch2D::addShape(const Shape& shape)
	{
		PK_ASSERT(m_isValid, "Trying to add a shape to a RenderBatch2D that is not yet created.", "Pekan");

		// Get shape's vertices
		const Vertex2D* vertices = shape.getVertices(float(m_colorsCount));
		const int verticesCount = shape.getVerticesCount();
		// Get shape's indices
		const unsigned* zeroBasedIndices = shape.getIndices();
		const int indicesCount = shape.getIndicesCount();
		// Get shape's color
		glm::vec4 color = shape.getColor();

		// If adding this shape would overflow the batch, don't add it
		if (wouldShapeOverflowBatch(verticesCount, indicesCount))
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

		// Add shape's color to the batch
		m_colors.push_back(color);
		m_colorsCount++;

		return true;
	}

	bool RenderBatch2D::addSprite(const Sprite& sprite)
	{
		PK_ASSERT(m_isValid, "Trying to add a sprite to a RenderBatch2D that is not yet created.", "Pekan");

		// If adding this sprite would overflow the batch, don't add it.
		if (wouldSpriteOverflowBatch())
		{
			return false;
		}

		const unsigned oldVerticesSize = unsigned(m_vertices.size());
		const size_t oldIndicesSize = m_indices.size();

		// Get sprite's vertices
		const Vertex2D* vertices = sprite.getVertices(float(m_textures.size()));
		// Add sprite's vertices to the batch
		m_vertices.insert(m_vertices.end(), vertices, vertices + 4);

		// Add sprites's indices { 0, 1, 2, 0, 2, 3 } to the batch,
		// first making each index relative to where sprite's vertices begin in the vertices list.
		// Sprite's indices are "zero based" meaning they are relative to 0, starting at 0.
		// That's because internally sprite's vertices also start at 0,
		// but now that we added them to the vertices list they no longer start at 0.
		// They start at however many vertices there were in the list before adding them,
		// so that's why we add oldVerticesSize to each index.
		m_indices.resize(m_indices.size() + 6);
		m_indices[oldIndicesSize + 0] = oldVerticesSize + 0;
		m_indices[oldIndicesSize + 1] = oldVerticesSize + 1;
		m_indices[oldIndicesSize + 2] = oldVerticesSize + 2;
		m_indices[oldIndicesSize + 3] = oldVerticesSize + 0;
		m_indices[oldIndicesSize + 4] = oldVerticesSize + 2;
		m_indices[oldIndicesSize + 5] = oldVerticesSize + 3;

		// Get sprite's textures
		const Texture2D_ConstPtr& texture = sprite.getTexture();
		// Add sprite's texture to the batch
		m_textures.push_back(texture);

		return true;
	}

	void RenderBatch2D::render(const Camera2D_ConstPtr& camera)
	{
		PK_ASSERT(m_isValid, "Trying to render a RenderBatch2D that is not yet created.", "Pekan");

		// Set underlying render object's vertex data and index data
		// to the data of our vertices list and indices list
		m_renderObject.setVertexData(m_vertices.data(), m_vertices.size() * sizeof(Vertex2D));
		m_renderObject.setIndexData(m_indices.data(), m_indices.size() * sizeof(unsigned));

		// Set the colors of the underlying colors texture to our list of colors
		m_colorsTexture.setColors(m_colors);
		// Bind colors texture to slot 0
		m_colorsTexture.bind(0);

		Shader& shader = m_renderObject.getShader();
		setViewProjectionMatrixUniform(shader, camera);
		// Set shader's "uColorsTexture" uniform to be 0, matching the slot where colors texture is bound.
		shader.setUniform1i("uColorsTexture", 0);
		// Set shader's "uColorsCount" uniform to be the number of colors in the batch.
		shader.setUniform1i("uColorsCount", m_colorsCount);
		// Bind all textures to slots 1, 2, 3, ...
		// Starting at 1 because slot 0 is occupied by the 1D colors texture.
		for (unsigned i = 0; i < m_textures.size(); i++)
		{
			m_textures[i]->bind(i + 1);
		}

		// Set the value of "uTextures" uniform inside the shader
		setTexturesUniform(shader, size_t(m_capacityTextures));

		// Render the underlying render object, drawing all triangles making up all primitives from the batch
		m_renderObject.render();
	}

	void RenderBatch2D::clear()
	{
		PK_ASSERT(m_isValid, "Trying to clear a RenderBatch2D that is not yet created.", "Pekan");

		m_vertices.clear();
		m_indices.clear();
		m_textures.clear();
		m_colors.clear();

		m_colorsCount = 0;
	}

	bool RenderBatch2D::wouldShapeOverflowBatch(int verticesCount, int indicesCount) const
	{
		return (m_colors.size() + 1 > m_capacityColors);
	}

	bool RenderBatch2D::wouldSpriteOverflowBatch() const
	{
		return (m_textures.size() + 1 > m_capacityTextures);
	}

} // namespace Renderer2D
} // namespace Pekan