#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

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

		// Creates a render object with vertices, indices and a shader.
		void create
		(
			const void* vertexData,
			long long vertexDataSize,
			const VertexBufferLayout& layout,
			BufferDataUsage vertexDataUsage,
			const void* indexData,
			long long indexDataSize,
			BufferDataUsage indexDataUsage,
			const char* vertexShaderSource,
			const char* fragmentShaderSource
		);

		// Creates a render object with vertices and a shader.
		// Index data can be set later.
		void create
		(
			const void* vertexData,
			long long vertexDataSize,
			const VertexBufferLayout& layout,
			BufferDataUsage vertexDataUsage,
			const char* vertexShaderSource,
			const char* fragmentShaderSource
		);

		// Creates a render object with a shader only.
		// Vertex data and index data can be set later.
		void create
		(
			const VertexBufferLayout& layout,
			const char* vertexShaderSource,
			const char* fragmentShaderSource
		);

		void destroy();

		void bind();
		void unbind();

		void setVertexData
		(
			const void* data,
			long long size
		);
		void setVertexData
		(
			const void* data,
			long long size,
			BufferDataUsage dataUsage
		);

		void setIndexData
		(
			const void* data,
			long long size
		);
		void setIndexData
		(
			const void* data,
			long long size,
			BufferDataUsage dataUsage
		);

		void setTextureImage
		(
			const Image& image,
			const char* uniformName,
			unsigned slot
		);

		Shader& getShader() { return m_shader; }
		const Shader& getShader() const { return m_shader; }

	private:

		VertexArray m_vertexArray;

		VertexBuffer m_vertexBuffer;

		IndexBuffer m_indexBuffer;

		Shader m_shader;

		Texture m_texture;
		unsigned m_textureSlot = 0xffffffff;

		BufferDataUsage m_vertexDataUsage = BufferDataUsage::None;
		BufferDataUsage m_indexDataUsage = BufferDataUsage::None;
	};

} // namespace Renderer
} // namespace Pekan