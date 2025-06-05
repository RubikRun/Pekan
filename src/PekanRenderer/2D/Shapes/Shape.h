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

		virtual int getNumberOfVertices() const = 0;

	private: /* functions */

		virtual void _moveVertices(glm::vec2 deltaPosition) = 0;

		virtual const glm::vec2* getVertexData() const = 0;

		// Can be overriden by derived classes to return the desired draw mode to be used for rendering the shape
		virtual DrawMode getDrawMode() const { return DrawMode::Triangles; }

		inline int getVertexDataSize() const { return getNumberOfVertices() * sizeof(float) * 2; }
		inline int getIndexDataSize() const { return (getNumberOfVertices() - 2) * 3 * sizeof(unsigned); }

	protected: /* functions */

		// Creates the underlying render object with given vertex data
		// @param[in] dynamic - Specifies if shape is going to be moved often. Used for optimization.
		void createRenderObject(const void* vertexData, bool dynamic);

		// Creates the underlying render object with given vertex data and index data
		// @param[in] dynamic - Specifies if shape is going to be moved often. Used for optimization.
		void createRenderObject(const void* vertexData, const void* indexData, bool dynamic);

		// Updates the underlying render object with current vertex data
		void updateRenderObject();

		// Updates the underlying render object with current vertex data and given index data
		void updateRenderObject(const void* indexData);

	protected: /* variables*/

		// Shape's position (origin) in world space.
		// 
		// Vertices will be relative to this position
		// so that the whole shape can be moved by changing the position.
		glm::vec2 m_position = glm::vec2(0.0f, 0.0f);

	private: /* variables */

		RenderObject m_renderObject;

		glm::vec4 m_color;

		bool m_usingIndices = false;
	};

} // namespace Renderer
} // namespace Renderer