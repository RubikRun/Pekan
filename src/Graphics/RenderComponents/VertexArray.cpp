#include "VertexArray.h"

#include "PekanLogger.h"
#include "GLCall.h"
#include "RenderState.h"


namespace Pekan
{
namespace Graphics
{

	VertexArray::~VertexArray()
	{
		PK_ASSERT(!isValid(), "You forgot to destroy() a VertexArray instance.", "Pekan");
	}

	void VertexArray::create()
	{
		PK_ASSERT(!isValid(), "Trying to create a VertexArray instance that is already created.", "Pekan");

		GLCall(glGenVertexArrays(1, &m_id));
		bind();
	}

	void VertexArray::destroy()
	{
		PK_ASSERT(isValid(), "Trying to destroy a VertexArray instance that is not yet created.", "Pekan");

		GLCall(glDeleteVertexArrays(1, &m_id));
		m_id = 0;

		m_vertexBuffers.clear();
	}

	void VertexArray::bind() const
	{
		PK_ASSERT(isValid(), "Trying to bind a VertexArray that is not yet created.", "Pekan");

		GLCall(glBindVertexArray(m_id));
	}

	void VertexArray::unbind() const
	{
		PK_ASSERT(isValid(), "Trying to unbind a VertexArray that is not yet created.", "Pekan");

		GLCall(glBindVertexArray(0));
	}

	void VertexArray::addVertexBuffer(VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
	{
		PK_ASSERT(isValid(), "Trying to add a VertexBuffer to a VertexArray that is not yet created.", "Pekan");

		if (!vertexBuffer.isValid())
		{
			PK_LOG_ERROR("Trying to add an invalid VertexBuffer to a VertexArray. It will not be added.", "Pekan");
			return;
		}
		if (layout.getElements().empty())
		{
			PK_LOG_ERROR("Trying to add a VertexBuffer with an empty layout to a VertexArray. It will not be added.", "Pekan");
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

} // namespace Graphics
} // namespace Pekan