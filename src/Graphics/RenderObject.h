#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture2D.h"
#include "RenderCommands.h"

#include <vector>

namespace Pekan
{
namespace Graphics
{

	// A class representing a render object in Pekan.
	// A render object in Pekan is a standard set of render components needed to render something: 
	// - vertices
	// - indices (optional)
	// - a shader
	// - textures (optional)
	class RenderObject
	{
	public:

		// Creates a render object with vertices and a shader.
		// Index data and textures can be provided later.
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
		// Vertex data, index data and textures can be provided later.
		void create
		(
			const VertexBufferLayout& layout,
			const char* vertexShaderSource,
			const char* fragmentShaderSource
		);
		void destroy();

		// Renders the object
		void render(DrawMode mode = DrawMode::Triangles) const;

		// Sets new vertex data to the render object (old data usage will be used)
		void setVertexData(const void* data, long long size);
		// Sets new vertex data, with a new data usage, to the render object
		void setVertexData(const void* data, long long size, BufferDataUsage dataUsage);
		// Fills a region of render object's vertex data with given data. Previous data in this region is overwritten.
		void setVertexSubData(const void* data, long long offset, long long size);

		// Sets new index data to the render object (old data usage will be used)
		void setIndexData(const void* data, long long size);
		// Sets new index data, with a new data usage, to the render object
		void setIndexData(const void* data, long long size, BufferDataUsage dataUsage);
		// Fills a region of render object's index data with given data. Previous data in this region is overwritten.
		void setIndexSubData(const void* data, long long offset, long long size);

		// Sets new source code to be used for render object's shader
		void setShaderSource(const char* vertexShaderSource, const char* fragmentShaderSource);

		// Sets an image to be used as a texture inside render object's shader.
		// @param[in] uniformName - Name of uniform containing the texture inside the shader
		// @param[in] slot - Slot where texture will be bound
		void setTextureImage(const Image& image, const char* uniformName, unsigned slot);

		Shader& getShader() { return m_shader; }
		const Shader& getShader() const { return m_shader; }

		// Checks if render object is valid, meaning that it has been successfully created and not yet destroyed.
		bool isValid() const;

	private: /* functions */

		void bind() const;
		void unbind() const;

		// Clears all textures of render object, leaving it without textures
		void clearTextures();

	private: /* variables */

		VertexArray m_vertexArray;

		VertexBuffer m_vertexBuffer;

		IndexBuffer m_indexBuffer;

		Shader m_shader;

		// Layout of the vertex buffer
		VertexBufferLayout m_vertexBufferLayout;

		// A vector of textures, where the index of each texture is the slot where it will be bound
		std::vector<Texture2D_Ptr> m_textures;

		// Data usage of the vertex buffer
		BufferDataUsage m_vertexDataUsage = BufferDataUsage::None;
		// Data usage of the index buffer
		BufferDataUsage m_indexDataUsage = BufferDataUsage::None;

		// A flag indicating if render object is valid, meaning that it has been successfully created and not yet destroyed.
		bool m_isValid = false;
	};

} // namespace Graphics
} // namespace Pekan
