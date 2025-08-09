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

	// Maximum allowed number of vertices per batch.
	static constexpr size_t MAX_VERTICES_PER_BATCH = 37;
	// Maximum allowed number of indices per batch.
	static constexpr size_t MAX_INDICES_PER_BATCH = 15000;

	// Sets "uTextures" uniform inside a given shader
	// to a list of texture slots { 0, 1, 2, 3, ... }
	// with as many texture slots as are supported on current hardware.
	static void setTexturesUniformValue(Shader& shader)
	{
		static const int maxTextureSlots = RenderState::getMaxTextureSlots();
		std::vector<int> textures(maxTextureSlots);
		for (size_t i = 0; i < maxTextureSlots; i++)
		{
			textures[i] = i;
		}

		// TODO: temp
		PK_ASSERT(textures.size() == 32, "(TEMP) Maximum number of texture slots on your hardware differs from the one hardcoded in Sprite_FragmentShader.glsl", "Pekan");

		shader.setUniform1iv("uTextures", textures.size(), textures.data());
	}

	void SpritesBatch::create(BufferDataUsage bufferDataUsage)
	{
		PK_ASSERT(!m_isValid, "Trying to create a SpritesBatch instance that is already created.", "Pekan");

		// Create underlying render object with empty vertex data
		m_renderObject.create
		(
			nullptr,
			MAX_VERTICES_PER_BATCH * sizeof(SpriteVertex),
			{ { ShaderDataType::Float2, "position" }, { ShaderDataType::Float2, "textureCoordinates" }, { ShaderDataType::Float, "textureIndex" }},
			bufferDataUsage,
			FileUtils::readFileToString(VERTEX_SHADER_FILEPATH).c_str(),
			FileUtils::readFileToString(FRAGMENT_SHADER_FILEPATH).c_str()
		);
		// and empty index data
		// (we need to explicitly set empty index data because we are also setting data usage)
		m_renderObject.setIndexData(nullptr, MAX_INDICES_PER_BATCH * sizeof(unsigned), bufferDataUsage);

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

		static const int maxTextureSlots = RenderState::getMaxTextureSlots();

		//////// Handle carry-over
		{
			// If we have carry-over sprites from previous batch, add them to this batch
			if (!m_carryOverVertices.empty())
			{
				// Make sure we have all carry-over components
				PK_ASSERT(!m_carryOverIndices.empty() && !m_carryOverTextures.empty(),
					"There are carry-over vertices in SpritesBatch but there are no carry-over indices or no carry-over textures.", "Pekan");

				// Add carry-over vertices to the main list of vertices
				m_vertices.insert(m_vertices.end(), m_carryOverVertices.begin(), m_carryOverVertices.end());
				PK_ASSERT_QUICK(m_vertices.size() <= MAX_VERTICES_PER_BATCH);
				m_carryOverVertices.clear();
				// Add carry-over indices to the main list of indices
				m_indices.insert(m_indices.end(), m_carryOverIndices.begin(), m_carryOverIndices.end());
				PK_ASSERT_QUICK(m_indices.size() <= MAX_INDICES_PER_BATCH);
				m_carryOverIndices.clear();
				// Add carry-over textures to the main list of textures
				m_textures.insert(m_textures.end(), m_carryOverTextures.begin(), m_carryOverTextures.end());
				PK_ASSERT_QUICK(m_textures.size() <= maxTextureSlots);
				m_carryOverTextures.clear();
			}
			else
			{
				// Make sure that if we don't have carry-over vertices we also don't have carry-over indices or textures
				PK_ASSERT(m_carryOverIndices.empty() && m_carryOverTextures.empty(),
					"There are no carry-over vertices in SpritesBatch but there are carry-over indices or carry-over textures", "Pekan");
			}
		}

		const unsigned oldVerticesSize = unsigned(m_vertices.size());

		// A flag indicating if sprite will be added to the carry-over lists of vertices, indices and textures
		const bool doCarryOver = (m_vertices.size() + 4 > MAX_VERTICES_PER_BATCH || m_indices.size() + 6 > MAX_INDICES_PER_BATCH);

		// A flag indicating if more sprites can be added after this one,
		// or the batch needs to be rendered, and a new one started.
		bool canAddMore = !doCarryOver;

		// Set sprite's texture's index,
		// letting the sprite know what its texture index is inside of the batch,
		// so that it can set the "textureIndex" attribute to its vertices.
		sprite.setTextureIndex(float(m_textures.size()));

		//////// Handle vertices
		{
			// Get sprite's vertices
			const SpriteVertex* vertices = sprite.getVertices();

			// Add sprite's vertices to the batch,
			// either to the carry-over list of vertices or the main list of vertices.
			if (doCarryOver)
			{
				m_carryOverVertices.insert(m_carryOverVertices.end(), vertices, vertices + 4);
			}
			else
			{
				m_vertices.insert(m_vertices.end(), vertices, vertices + 4);
				// If we reach the maximum number of vertices, we can't add more sprites
				if (m_vertices.size() >= MAX_VERTICES_PER_BATCH)
				{
					canAddMore = false;
				}
			}
		}

		//////// Handle indices
		{
			// Add sprites's indices { 0, 1, 2, 0, 2, 3 } to the batch,
			// either to the carry-over list of indices or the main list of indices,
			// first making each index relative to where sprite's vertices begin in the vertices list.
			// Sprite's indices are "zero based" meaning they are relative to 0, starting at 0.
			// That's because internally sprite's vertices also start at 0,
			// but now that we added them to the vertices list they no longer start at 0.
			// They start at however many vertices there were in the list before adding them,
			// so that's why we add oldVerticesSize to each index.
			if (doCarryOver)
			{
				const size_t oldCarryOverIndicesSize = m_carryOverIndices.size();
				m_carryOverIndices.resize(m_carryOverIndices.size() + 6);
				m_carryOverIndices[oldCarryOverIndicesSize + 0] = oldVerticesSize + 0;
				m_carryOverIndices[oldCarryOverIndicesSize + 1] = oldVerticesSize + 1;
				m_carryOverIndices[oldCarryOverIndicesSize + 2] = oldVerticesSize + 2;
				m_carryOverIndices[oldCarryOverIndicesSize + 3] = oldVerticesSize + 0;
				m_carryOverIndices[oldCarryOverIndicesSize + 4] = oldVerticesSize + 2;
				m_carryOverIndices[oldCarryOverIndicesSize + 5] = oldVerticesSize + 3;
			}
			else
			{
				const size_t oldIndicesSize = m_indices.size();
				m_indices.resize(m_indices.size() + 6);
				m_indices[oldIndicesSize + 0] = oldVerticesSize + 0;
				m_indices[oldIndicesSize + 1] = oldVerticesSize + 1;
				m_indices[oldIndicesSize + 2] = oldVerticesSize + 2;
				m_indices[oldIndicesSize + 3] = oldVerticesSize + 0;
				m_indices[oldIndicesSize + 4] = oldVerticesSize + 2;
				m_indices[oldIndicesSize + 5] = oldVerticesSize + 3;
				// If we reach the maximum number of indices, we can't add more sprites
				if (m_indices.size() >= MAX_INDICES_PER_BATCH)
				{
					canAddMore = false;
				}
			}
		}

		//////// Handle texture
		{
			// Get sprite's texture
			const Texture2D_ConstPtr& texture = sprite.getTexture();
			// Add sprite's texture to the batch,
			// either to the carry-over list of textures or the main list of textures.
			if (doCarryOver)
			{
				m_carryOverTextures.push_back(texture);
			}
			else
			{
				m_textures.push_back(texture);
				// If we reach the maximum number of texture slots, we can't add more sprites
				if (m_textures.size() >= maxTextureSlots)
				{
					canAddMore = false;
				}
			}
		}

		return canAddMore;
	}

	void SpritesBatch::render(const Camera2D_ConstPtr& camera, bool isFinal)
	{
		PK_ASSERT(m_isValid, "Trying to render a SpritesBatch that is not yet created.", "Pekan");

		// If camera is null, render without a camera
		if (camera == nullptr)
		{
			PK_ASSERT(false, "Trying to render a SpritesBatch with a null camera.", "Pekan");
			render();
			return;
		}

		// If this is the final render() call for this frame,
		// we need to render carry-over sprites as well.
		if (isFinal)
		{
			renderCarryOver(camera);
		}

		// If there are no vertices to render, we can finish early.
		if (m_vertices.empty())
		{
			PK_ASSERT_QUICK(m_indices.empty() && m_textures.empty());
			return;
		}

		// Set underlying render object's vertex data and index data
		// to the data of our vertices list and indices list
		m_renderObject.setVertexSubData(m_vertices.data(), 0, m_vertices.size() * sizeof(SpriteVertex));
		m_renderObject.setIndexSubData(m_indices.data(), 0, m_indices.size() * sizeof(unsigned));

		Shader& shader = m_renderObject.getShader();

		// Set shader's view projection matrix uniform to camera's transform
		const glm::mat4& viewProjectionMatrix = camera->getViewProjectionMatrix();
		shader.setUniformMatrix4fv("uViewProjectionMatrix", viewProjectionMatrix);

		// Bind all textures
		for (unsigned i = 0; i < m_textures.size(); i++)
		{
			m_textures[i]->bind(i);
		}

		// Set the value of "uTextures" uniform inside the shader
		setTexturesUniformValue(shader);

		// Draw all triangles making up all sprites from the batch
		RenderCommands::drawIndexed(m_indices.size());
	}

	void SpritesBatch::render(bool isFinal)
	{
		PK_ASSERT(m_isValid, "Trying to render a SpritesBatch that is not yet created.", "Pekan");

		// If this is the final render() call for this frame,
		// we need to render carry-over sprites as well.
		if (isFinal)
		{
			renderCarryOver();
		}

		// If there are no vertices to render, we can finish early.
		if (m_vertices.empty())
		{
			PK_ASSERT_QUICK(m_indices.empty() && m_textures.empty());
			return;
		}

		// Set underlying render object's vertex data and index data
		// to the data of our vertices list and indices list
		m_renderObject.setVertexSubData(m_vertices.data(), 0, m_vertices.size() * sizeof(SpriteVertex));
		m_renderObject.setIndexSubData(m_indices.data(), 0, m_indices.size() * sizeof(unsigned));

		Shader& shader = m_renderObject.getShader();

		// Set shader's view projection matrix uniform to a default view projection matrix
		static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
		shader.setUniformMatrix4fv("uViewProjectionMatrix", defaultViewProjectionMatrix);

		// Bind all textures
		for (unsigned i = 0; i < m_textures.size(); i++)
		{
			m_textures[i]->bind(i);
		}

		// Set the value of "uTextures" uniform inside the shader
		setTexturesUniformValue(shader);

		// Draw all triangles making up all sprites from the batch
		RenderCommands::drawIndexed(m_indices.size());
	}

	void SpritesBatch::clear(bool isFinal)
	{
		PK_ASSERT(m_isValid, "Trying to clear a SpritesBatch that is not yet created.", "Pekan");

		m_vertices.clear();
		m_indices.clear();
		m_textures.clear();

		// If this is the final call to clear() for this frame, we need to clear carry-over lists as well
		if (isFinal)
		{
			m_carryOverVertices.clear();
			m_carryOverIndices.clear();
			m_carryOverTextures.clear();
		}
	}

	void SpritesBatch::renderCarryOver(const Camera2D_ConstPtr& camera)
	{
		PK_ASSERT(m_isValid, "Trying to render carry-over sprites from a SpritesBatch that is not yet created.", "Pekan");

		// If camera is null, render without a camera
		if (camera == nullptr)
		{
			PK_ASSERT(false, "Trying to render carry-over sprites from a SpritesBatch with a null camera.", "Pekan");
			renderCarryOver();
			return;
		}

		// If there are no carry-over vertices to render, we can finish early.
		if (m_carryOverVertices.empty())
		{
			PK_ASSERT_QUICK(m_carryOverIndices.empty() && m_carryOverTextures.empty());
			return;
		}

		// Set underlying render object's vertex data and index data
		// to the data of our carry-over vertices list and carry-over indices list
		m_renderObject.setVertexSubData(m_carryOverVertices.data(), 0, m_carryOverVertices.size() * sizeof(SpriteVertex));
		m_renderObject.setIndexSubData(m_carryOverIndices.data(), 0, m_carryOverIndices.size() * sizeof(unsigned));

		Shader& shader = m_renderObject.getShader();

		// Set shader's view projection matrix uniform to camera's transform
		const glm::mat4& viewProjectionMatrix = camera->getViewProjectionMatrix();
		shader.setUniformMatrix4fv("uViewProjectionMatrix", viewProjectionMatrix);

		// Bind all carry-over textures
		for (unsigned i = 0; i < m_carryOverTextures.size(); i++)
		{
			m_carryOverTextures[i]->bind(i);
		}

		// Set the value of "uTextures" uniform inside the shader
		setTexturesUniformValue(shader);

		// Draw all triangles making up all carry-over sprites from the batch
		RenderCommands::drawIndexed(m_indices.size());
	}

	void SpritesBatch::renderCarryOver()
	{
		PK_ASSERT(m_isValid, "Trying to render carry-over sprites from a SpritesBatch that is not yet created.", "Pekan");

		// If there are no carry-over vertices to render, we can finish early.
		if (m_carryOverVertices.empty())
		{
			PK_ASSERT_QUICK(m_carryOverIndices.empty() && m_carryOverTextures.empty());
			return;
		}

		// Set underlying render object's vertex data and index data
		// to the data of our carry-over vertices list and carry-over indices list
		m_renderObject.setVertexSubData(m_carryOverVertices.data(), 0, m_carryOverVertices.size() * sizeof(SpriteVertex));
		m_renderObject.setIndexSubData(m_carryOverIndices.data(), 0, m_carryOverIndices.size() * sizeof(unsigned));

		Shader& shader = m_renderObject.getShader();

		// Set shader's view projection matrix uniform to a default view projection matrix
		static const glm::mat4 defaultViewProjectionMatrix = glm::mat4(1.0f);
		shader.setUniformMatrix4fv("uViewProjectionMatrix", defaultViewProjectionMatrix);

		// Bind all carry-over textures
		for (unsigned i = 0; i < m_carryOverTextures.size(); i++)
		{
			m_carryOverTextures[i]->bind(i);
		}

		// Set the value of "uTextures" uniform inside the shader
		setTexturesUniformValue(shader);

		// Draw all triangles making up all carry-over sprites from the batch
		RenderCommands::drawIndexed(m_indices.size());
	}

} // namespace Renderer2D
} // namespace Pekan