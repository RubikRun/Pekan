#pragma once

#include "RenderObject.h"
#include "Camera2D.h"

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer
{

	class Shape
	{
	public:

		void render() const;
		// Renders shape using the given camera
		void render(const Camera2D& camera);

		void setPosition(glm::vec2 position);
		void move(glm::vec2 deltaPosition);
		void setColor(glm::vec4 color);

		inline glm::vec2 getPosition() const { return m_position; }
		inline glm::vec4 getColor() const { return m_color; }

		virtual int getNumberOfVertices() const = 0;

		// Checks if shape is valid,
		// meaning it has been created and not yet destroyed.
		//
		// NOTE: Derived classes have the responsibility to create the underlying render object
		//       in their create() functions and that's when a shape becomes valid,
		//       NOT when Shape::create() is called.
		inline bool isValid() const { return m_renderObject.isValid(); }

	private: /* functions */

		virtual void _moveVertices(glm::vec2 deltaPosition) = 0;

		virtual const glm::vec2* getVertexData() const = 0;
		virtual const unsigned* getIndexData() const { return nullptr; }

		// Can be overriden by derived classes to return the desired draw mode to be used for rendering the shape
		virtual DrawMode getDrawMode() const { return DrawMode::Triangles; }

		inline int getVertexDataSize() const { return getNumberOfVertices() * sizeof(float) * 2; }
		inline int getIndexDataSize() const { return (getNumberOfVertices() - 2) * 3 * sizeof(unsigned); }

	protected: /* functions */

		// Creates the underlying render object
		// @param[in] dynamic - Specifies if shape is going to be moved often. Used for optimization.
		void createRenderObject(bool dynamic);
		void destroyRenderObject();

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

		glm::vec4 m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		bool m_usingIndices = false;
	};

} // namespace Renderer
} // namespace Renderer