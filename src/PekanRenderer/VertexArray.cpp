#include "VertexArray.h"
#define PK_FILENAME "VertexArray.cpp"
#include "Logger/PekanLogger.h"
#include "PekanRenderer.h"

#include <glad/glad.h>

namespace Pekan
{
namespace Renderer
{

	VertexArray::~VertexArray()
	{
		destroy();
	}

	void VertexArray::create()
	{
		glGenVertexArrays(1, &id);
		bind();
	}

	void VertexArray::destroy() {
		unbind();
		glDeleteVertexArrays(1, &id);
		id = 0;
		vertexBuffers.clear();
	}

	void VertexArray::bind() const {
		glBindVertexArray(id);
	}

	void VertexArray::unbind() const {
		glBindVertexArray(0);
	}

	void VertexArray::addVertexBuffer(const VertexBuffer& vertexBuffer) {
		if (!vertexBuffer.isValid())
		{
			PK_LOG_ERRORF("Trying to add an invalid vertex buffer to a vertex array. It will not be added.");
			return;
		}
		const VertexBufferLayout& layout = vertexBuffer.getLayout();
		if (layout.getElements().empty())
		{
			PK_LOG_ERRORF("Trying to add a vertex buffer with an empty layout to a vertex array. It will not be added.");
			return;
		}

		const std::vector<VertexBufferElement>& layoutElements = layout.getElements();
		// Bind the vertex array and the vertex buffer
		bind();
		vertexBuffer.bind();
		// Traverse layout's elements
		for (int i = 0; i < layoutElements.size(); ++i) {
			const VertexBufferElement& element = layoutElements[i];
			// For each element, enable and configure a vertex attribute
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(
				i,
				element.getComponentsCount(),
				PekanRenderer::getShaderDataTypeOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				reinterpret_cast<GLvoid*>((long long)(element.getOffset()))
			);
		}
		// Push the vertex buffer to the vector of vertex buffers inside the vertex array
		vertexBuffers.push_back(vertexBuffer);
	}


} // namespace Renderer
} // namespace Pekan