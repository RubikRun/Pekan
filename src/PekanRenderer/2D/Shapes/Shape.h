#pragma once

#include "RenderObject.h"

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer
{

	class Shape
	{
	public:

		void create();

		void destroy();

		void render() const;

		inline glm::vec2 getPosition() const { return m_position; }

		void setPosition(glm::vec2 position);
		void move(glm::vec2 deltaPosition);

		inline glm::vec4 getColor() const { return m_color; }
		void setColor(glm::vec4 color);

		inline int getVerticesSize() const { return getNumberOfVertices() * sizeof(float) * 2; }

	private: /* functions */

		virtual void _moveVertices(glm::vec2 deltaPosition) = 0;

		virtual int getNumberOfVertices() const = 0;

	protected: /* functions */

		// Creates the underlying render object with given vertex data
		// @param[in] dynamic - Specifies if shape is going to be moved often. Used for optimization.
		void createRenderObject(const void* vertexData, bool dynamic);

	protected: /* variables*/

		RenderObject m_renderObject;

		// Shape's position (origin) in world space.
		// 
		// Vertices will be relative to this position
		// so that the whole shape can be moved by changing the position.
		glm::vec2 m_position = glm::vec2(0.0f, 0.0f);

		glm::vec4 m_color;
	};

} // namespace Renderer
} // namespace Renderer