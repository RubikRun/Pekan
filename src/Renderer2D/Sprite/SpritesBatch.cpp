#include "SpritesBatch.h"

#include "PekanLogger.h"
#include "Utils/FileUtils.h"
#include "RenderCommands.h"

using namespace Pekan::Graphics;

#define VERTEX_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Sprite/Shaders/Sprite_VertexShader.glsl"
#define FRAGMENT_SHADER_FILEPATH PEKAN_RENDERER2D_ROOT_DIR "/Sprite/Shaders/Sprite_FragmentShader.glsl"

namespace Pekan
{
namespace Renderer2D
{

	// Sets "uTextures" uniform inside a given shader
	// to a list of texture slots { 0, 1, 2, 3, ... }
	// with as many texture slots as is batch's capacity.
	static void setTexturesUniform(Shader& shader, size_t capacityTextures)
	{
		std::vector<int> textures(capacityTextures);
		for (size_t i = 0; i < capacityTextures; i++)
		{
			textures[i] = int(i);
		}

		// TODO: temp
		PK_ASSERT(textures.size() == 32, "(TEMP) Maximum number of texture slots on your hardware differs from the one hardcoded in Sprite_FragmentShader.glsl", "Pekan");

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
			static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
			shader.setUniformMatrix4fv("uViewProjectionMatrix", defaultViewProjectionMatrix);
		}
	}

	void SpritesBatch::create(BufferDataUsage bufferDataUsage)
	{
		PK_ASSERT(!m_isValid, "Trying to create a SpritesBatch instance that is already created.", "Pekan");

		// Set batch's capacity for textures to be the maximum number of texture slots supported on current hardware
		m_capacityTextures = RenderState::getMaxTextureSlots();
		// Since each sprite has exactly 4 vertices, 6 indices and 1 texture,
		// we can calculate vertices capacity and indices capacity based on the textures capacity
		m_capacityVertices = m_capacityTextures * 4;
		m_capacityIndices = m_capacityTextures * 6;

		// Create underlying render object, pre-allocating memory for vertex data
		m_renderObject.create
		(
			nullptr,
			m_capacityVertices * sizeof(SpriteVertex),
			{ { ShaderDataType::Float2, "position" }, { ShaderDataType::Float2, "textureCoordinates" }, { ShaderDataType::Float, "textureIndex" }},
			bufferDataUsage,
			FileUtils::readFileToString(VERTEX_SHADER_FILEPATH).c_str(),
			FileUtils::readFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
		);
		// Pre-allocate memory for index data
		m_renderObject.setIndexData(nullptr, m_capacityIndices * sizeof(unsigned), bufferDataUsage);

		// Set shader's view projection matrix uniform to a default view projection matrix
		static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
		m_renderObject.getShader().setUniformMatrix4fv("uViewProjectionMatrix", defaultViewProjectionMatrix);

		m_isValid = true;
	}

	void SpritesBatch::destroy()
	{
		PK_ASSERT(m_isValid, "Trying to destroy a SpritesBatch instance that is not yet created.", "Pekan");

		m_renderObject.destroy();
		clear();

		m_isValid = false;
	}

	bool SpritesBatch::addSprite(const Sprite& sprite)
	{
		PK_ASSERT(m_isValid, "Trying to add a sprite to a SpritesBatch that is not yet created.", "Pekan");

		// If adding this sprite would overflow the batch, don't add it.
		if (wouldOverflow())
		{
			return false;
		}

		const unsigned oldVerticesSize = unsigned(m_vertices.size());
		const size_t oldIndicesSize = m_indices.size();

		// Set sprite's texture's index,
		// letting the sprite know what its texture index is inside of the batch,
		// so that it can set the "textureIndex" attribute to its vertices.
		sprite.setTextureIndex(float(m_textures.size()));

		// Get sprite's vertices
		const SpriteVertex* vertices = sprite.getVertices();
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

	void SpritesBatch::render(const Camera2D_ConstPtr& camera)
	{
		PK_ASSERT(m_isValid, "Trying to render a SpritesBatch that is not yet created.", "Pekan");

		// Set underlying render object's vertex data and index data
		// to the data of our vertices list and indices list
		m_renderObject.setVertexSubData(m_vertices.data(), 0, m_vertices.size() * sizeof(SpriteVertex));
		m_renderObject.setIndexSubData(m_indices.data(), 0, m_indices.size() * sizeof(unsigned));

		Shader& shader = m_renderObject.getShader();
		setViewProjectionMatrixUniform(shader, camera);
		// Bind all textures
		for (unsigned i = 0; i < m_textures.size(); i++)
		{
			m_textures[i]->bind(i);
		}
		// Set the value of "uTextures" uniform inside the shader
		setTexturesUniform(shader, size_t(m_capacityTextures));

		// Draw all triangles making up all sprites from the batch
		RenderCommands::drawIndexed(m_indices.size());
	}

	void SpritesBatch::clear()
	{
		PK_ASSERT(m_isValid, "Trying to clear a SpritesBatch that is not yet created.", "Pekan");

		m_vertices.clear();
		m_indices.clear();
		m_textures.clear();
	}

	bool SpritesBatch::wouldOverflow() const
	{
		return
		(
			m_vertices.size() + 4 > m_capacityVertices
			|| m_indices.size() + 6 > m_capacityIndices
			|| m_textures.size() + 1 > m_capacityTextures
		);
	}

} // namespace Renderer2D
} // namespace Pekan