#pragma once

#include "RenderState.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture2D.h"

#include <vector>

namespace Pekan
{
namespace Renderer2D
{

	// A class representing a renderable object in Pekan, consisting of:
	// - vertices
	// - indices (optional)
	// - a shader
	// - textures (optional)
	class RenderObject
	{
	public:

		// Creates a render object with vertices and a shader.
		// Index data and textures can be configured later.
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
		// Vertex data, index data and textures can be configured later.
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

		// Sets new vertex data to the render object (old data usage will be used)
		void setVertexData(const void* data, long long size);
		// Sets new vertex data, with a new data usage, to the render object
		void setVertexData(const void* data, long long size, Graphics::BufferDataUsage dataUsage);
		// Fills a region of render object's vertex data with given data. Previous data in this region is overwritten.
		void setVertexSubData(const void* data, long long offset, long long size);

		// Sets new index data to the render object (old data usage will be used)
		void setIndexData(const void* data, long long size);
		// Sets new index data, with a new data usage, to the render object
		void setIndexData(const void* data, long long size, Graphics::BufferDataUsage dataUsage);
		// Fills a region of render object's index data with given data. Previous data in this region is overwritten.
		void setIndexSubData(const void* data, long long offset, long long size);

		// Sets new source code to be used for render object's shader
		void setShaderSource(const char* vertexShaderSource, const char* fragmentShaderSource);

		// Sets a new image to be used as a texture inside render object's shader.
		// @param[in] uniformName - Name of uniform inside the shader containing the slot where texture will be bound
		// @param[in] slot - Slot where texture will be bound
		void setTextureImage(const Graphics::Image& image, const char* uniformName, unsigned slot);

		Graphics::Shader& getShader() { return m_shader; }
		const Graphics::Shader& getShader() const { return m_shader; }

	private:

		Graphics::VertexArray m_vertexArray;

		Graphics::VertexBuffer m_vertexBuffer;

		Graphics::IndexBuffer m_indexBuffer;

		Graphics::Shader m_shader;

		// A vector of textures, where the index of each texture is the slot where it will be bound
		std::vector<Graphics::Texture2D_Ptr> m_textures;

		Graphics::BufferDataUsage m_vertexDataUsage = Graphics::BufferDataUsage::None;
		Graphics::BufferDataUsage m_indexDataUsage = Graphics::BufferDataUsage::None;
	};

} // namespace Renderer2D
} // namespace Pekan
