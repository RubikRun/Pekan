#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace Pekan
{
namespace Renderer
{

	// A class representing a renderable object in Pekan.
	// It consists of vertices, indices and a shader.
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
			long long size,
			BufferDataUsage dataUsage
		);

		void setIndexData
		(
			const void* data,
			long long size,
			BufferDataUsage dataUsage
		);

		Shader& getShader() { return m_shader; }
		const Shader& getShader() const { return m_shader; }

	private:

		VertexArray m_vertexArray;

		VertexBuffer m_vertexBuffer;

		IndexBuffer m_indexBuffer;

		Shader m_shader;
	};

} // namespace Renderer
} // namespace Pekan