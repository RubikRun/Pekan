#include "VertexArray.h"
#include "Logger/PekanLogger.h"
#include "RenderState.h"

#include <glad/glad.h>

namespace Pekan
{
namespace Renderer
{

	VertexArray::~VertexArray()
	{
		if (isValid())
		{
			destroy();
		}
	}

	void VertexArray::bind() const {
		GLCall(glBindVertexArray(m_id));
	}

	void VertexArray::unbind() const {
		GLCall(glBindVertexArray(0));
	}

	void VertexArray::addVertexBuffer(VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
	{
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
		for (int i = 0; i < layoutElements.size(); ++i)
		{
			const VertexBufferElement& element = layoutElements[i];
			// For each element, enable and configure a vertex attribute
			GLCall(glEnableVertexAttribArray(i));
			if (RenderState::isShaderDataTypeInt(layoutElements[i].type))
			{
				GLCall(glVertexAttribIPointer(
					i,
					element.getComponentsCount(),
					RenderState::getShaderDataTypeOpenGLBaseType(element.type),
					layout.getStride(),
					reinterpret_cast<GLvoid*>((long long)(element.getOffset()))
				));
			}
			else
			{
				GLCall(glVertexAttribPointer(
					i,
					element.getComponentsCount(),
					RenderState::getShaderDataTypeOpenGLBaseType(element.type),
					element.normalized ? GL_TRUE : GL_FALSE,
					layout.getStride(),
					reinterpret_cast<GLvoid*>((long long)(element.getOffset()))
				));
			}
		}
		// Add vertex buffer, together with its layout, to vertex array
		m_vertexBuffers.push_back(VertexBufferBinding(vertexBuffer, layout));
	}

	void VertexArray::_create()
	{
		GLCall(glGenVertexArrays(1, &m_id));
	}

	void VertexArray::_destroy()
	{
		GLCall(glDeleteVertexArrays(1, &m_id));
		m_vertexBuffers.clear();
	}


} // namespace Renderer
} // namespace Pekan