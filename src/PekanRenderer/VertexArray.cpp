#include "VertexArray.h"
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
		glGenVertexArrays(1, &m_id);
		bind();
	}

	void VertexArray::destroy() {
		unbind();
		glDeleteVertexArrays(1, &m_id);
		m_id = 0;
		m_vertexBuffers.clear();
	}

	void VertexArray::bind() const {
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind() const {
		glBindVertexArray(0);
	}

	void VertexArray::addVertexBuffer(VertexBuffer& vertexBuffer, const VertexBufferLayout& layout) {
		if (!vertexBuffer.isValid())
		{
			PK_LOG_ERROR("Trying to add an invalid vertex buffer to a vertex array. It will not be added.", "Pekan");
			return;
		}
		if (layout.getElements().empty())
		{
			PK_LOG_ERROR("Trying to add a vertex buffer with an empty layout to a vertex array. It will not be added.", "Pekan");
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
		// Add vertex buffer, together with its layout, to vertex array
		m_vertexBuffers.push_back(VertexBufferBinding(vertexBuffer, layout));
	}


} // namespace Renderer
} // namespace Pekan