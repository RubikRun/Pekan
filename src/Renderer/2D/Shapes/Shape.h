#pragma once

#include "RenderObject.h"
#include "RenderCommands.h"
#include "Camera2D.h"

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer
{

	class Shape
	{
	public:

		// Renders shape using the given camera
		void render(const Camera2D& camera);
		// Renders shape in default [-1, 1] space
		void render() const;

		void setPosition(glm::vec2 position);
		void setRotation(float rotation); // in radians
		void setScale(glm::vec2 scale);
		void setColor(glm::vec4 color);

		void move(glm::vec2 deltaPosition);

		inline glm::vec2 getPosition() const { return m_position; }
		inline float getRotation() const { return m_rotation; }
		inline glm::vec2 getScale() const { return m_scale; }
		inline glm::vec4 getColor() const { return m_color; }

		virtual int getNumberOfVertices() const = 0;

		// Checks if shape is valid,
		// meaning it has been created and not yet destroyed.
		//
		// NOTE: Derived classes have the responsibility to create the underlying render object
		//       in their own create() function and that's when a shape becomes valid.
		inline bool isValid() const { return m_renderObject.isValid(); }

	protected: /* functions */

		// Creates the underlying render object.
		// To be called by derived classes in their create() function
		// once they have setup their vertex data and their index data (if present).
		// @param[in] dynamic - Specifies if shape is going to be moved often. Used for optimization.
		void createRenderObject(bool dynamic);
		void destroyRenderObject();

		// Updates the underlying render object with current vertex data.
		// To be called by derived classes whenever vertex data changes.
		void updateRenderObject(bool doUpdateVertices = true, bool doUpdateIndices = true);

		// To be implemented by derived classes to update their transformed (world) vertices
		// with the current transform matrix and current local vertices, and then update the underlying render object.
		virtual void updateTransformedVertices() = 0;

	private: /* functions */

		// To be implemented by derived classes to return their vertex data in world space.
		virtual const glm::vec2* getVertexData() const = 0;
		// Can be overriden by derived classes to return their index data, if indices are used at all.
		virtual const unsigned* getIndexData() const { return nullptr; }

		// Can be overriden by derived classes to return the desired draw mode to be used for rendering the shape
		virtual DrawMode getDrawMode() const { return DrawMode::Triangles; }

		inline int getVertexDataSize() const { return getNumberOfVertices() * sizeof(float) * 2; }
		inline int getIndexDataSize() const { return (getNumberOfVertices() - 2) * 3 * sizeof(unsigned); }

		// Updates the transform matrix with current position, rotation and scale,
		// and then updates the transformed vertices calling updateTransformedVertices().
		void updateTransformMatrix();

	protected: /* variables*/

		glm::vec2 m_position = glm::vec2(0.0f, 0.0f);
		float m_rotation = 0.0f; // in radians
		glm::vec2 m_scale = glm::vec2(1.0f, 1.0f);

		// Full 2D transform matrix, containing position, rotation and scale
		glm::mat3 m_transformMatrix = glm::mat3(1.0f);

	private: /* variables */

		RenderObject m_renderObject;

		glm::vec4 m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		bool m_usingIndices = false;
	};

} // namespace Renderer
} // namespace Renderer