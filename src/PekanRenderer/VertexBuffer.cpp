#include "VertexBuffer.h"

#include <glad/glad.h>

namespace Pekan
{
namespace Renderer
{

	VertexBuffer::~VertexBuffer() {
		if (id != 0)
		{
			destroy();
		}
	}

	void VertexBuffer::create(const void* data, long long size) {
		glGenBuffers(1, &id);
		bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::destroy() {
		unbind();
		glDeleteBuffers(1, &id);
		id = 0;
	}

	void VertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	void VertexBuffer::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

} // namespace Renderer
} // namespace Pekan