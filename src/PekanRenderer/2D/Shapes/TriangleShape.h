#pragma once

#include "RenderObject.h"

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer
{

	class TriangleShape
	{
	public:

		void create();

		// Creates a triangle shape from given positions of 3 vertices.
		// @param[in] dynamic - Specifies if triangle is going to be moved often. Used for optimization.
		void create
		(
			glm::vec2 vertexA, glm::vec2 vertexB, glm::vec2 vertexC,
			bool dynamic = true
		);

		void destroy();

		void render() const;

		inline glm::vec2 getPosition() const { return m_position; }
		inline const glm::vec2* getVertices() const { return m_vertices; }
		inline glm::vec2 getVertexA() const { return m_vertices[0]; }
		inline glm::vec2 getVertexB() const { return m_vertices[1]; }
		inline glm::vec2 getVertexC() const { return m_vertices[2]; }

		// Sets triangle's position (origin).
		// Can be used to change the position of the whole triangle without changing each vertex individually.
		void setPosition(glm::vec2 position);
		void move(glm::vec2 deltaPosition);

		inline glm::vec4 getColor() const { return m_color; }
		void setColor(glm::vec4 color);

	private:

		RenderObject m_renderObject;

		// The 3 vertices of the triangle, in world space
		glm::vec2 m_vertices[3] = { glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f) };

		// Triangle's position (origin) in world space
		glm::vec2 m_position = glm::vec2(0.0f, 0.0f);

		glm::vec4 m_color;
	};

} // namespace Renderer
} // namespace Renderer