#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace Demo
{

	class RenderObject
	{
	public:

		void create(
			const void* vertexData,
			long long vertexDatasSize,
			Pekan::Renderer::BufferDataUsage vertexDataUsage,
			const Pekan::Renderer::VertexBufferLayout& layout,
			const void* indexData,
			long long indexDataSize,
			Pekan::Renderer::BufferDataUsage indexDataUsage,
			const char* vertexShaderSource,
			const char* fragmentShaderSource
		);
		void destroy();

		void bind();
		void unbind();

		void setVertexData(
			const void* data,
			long long size,
			Pekan::Renderer::BufferDataUsage dataUsage
		);

	private:

		Pekan::Renderer::VertexArray m_vertexArray;

		Pekan::Renderer::VertexBuffer m_vertexBuffer;

		Pekan::Renderer::IndexBuffer m_indexBuffer;

		Pekan::Renderer::Shader m_shader;
	};

} // namespace Demo