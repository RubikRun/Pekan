#pragma once

#include "RenderState.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include <vector>

namespace Pekan
{
namespace Renderer
{

	// A class representing a renderable object in Pekan, consisting of:
	// - vertices
	// - indices (optional)
	// - a shader
	// - a texture (optional)
	class RenderObject
	{
	public:

		// Creates a render object with vertices and a shader.
		// Index data and a texture can be set later.
		void create
		(
			const void* vertexData,
			long long vertexDataSize,
			const Graphics::VertexBufferLayout& layout,
			Graphics::BufferDataUsage vertexDataUsage,
			const char* vertexShaderSource,
			const char* fragmentShaderSource
		);

		// Creates a render object with a shader only.
		// Vertex data, index data and a texture can be set later.
		void create
		(
			const Graphics::VertexBufferLayout& layout,
			const char* vertexShaderSource,
			const char* fragmentShaderSource
		);

		// Checks if render object is valid,
		// meaning that it has been created and not yet destroyed.
		bool isValid() const;

		void destroy();

		void bind() const;
		void unbind() const;

		void setVertexData(const void* data, long long size);
		void setVertexData(const void* data, long long size, Graphics::BufferDataUsage dataUsage);
		void setVertexSubData(const void* data, long long offset, long long size);

		void setIndexData(const void* data, long long size);
		void setIndexData(const void* data, long long size, Graphics::BufferDataUsage dataUsage);
		void setIndexSubData(const void* data, long long offset, long long size);

		void setShaderSource(const char* vertexShaderSource, const char* fragmentShaderSource);

		void setTextureImage(const Graphics::Image& image, const char* uniformName, unsigned slot);

		Graphics::Shader& getShader() { return m_shader; }
		const Graphics::Shader& getShader() const { return m_shader; }

	private:

		Graphics::VertexArray m_vertexArray;

		Graphics::VertexBuffer m_vertexBuffer;

		Graphics::IndexBuffer m_indexBuffer;

		Graphics::Shader m_shader;

		// A vector of textures, where the index of each texture is the slot where it will be bound
		std::vector<Graphics::TexturePtr> m_textures;

		Graphics::BufferDataUsage m_vertexDataUsage = Graphics::BufferDataUsage::None;
		Graphics::BufferDataUsage m_indexDataUsage = Graphics::BufferDataUsage::None;
	};

} // namespace Renderer
} // namespace Pekan