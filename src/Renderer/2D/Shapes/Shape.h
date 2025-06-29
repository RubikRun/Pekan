#pragma once

#include "RenderObject.h"
#include "RenderCommands.h"
#include "Camera2D.h"

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer
{

	// A vertex of a 2D shape
	struct ShapeVertex
	{
		glm::vec2 position = glm::vec2(0.0f, 0.0f);
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	};

	// A base class for 2D shapes
	class Shape
	{
	public:

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

		// To be implemented by derived classes to return their vertex data in world space.
		virtual const ShapeVertex* getVertices() const = 0;
		// To be implemented by derived classes to return the number of their vertices.
		virtual int getVerticesCount() const = 0;

		// Can be overriden by derived classes to return their index data, if indices are used at all.
		virtual const unsigned* getIndices() const { return nullptr; }
		// Can be overriden by derived classes to return the number of their indices, if indices are used at all.
		virtual int getIndicesCount() const { return 0; }

	protected: /* functions */

		void create();
		void destroy();

		// Checks if shape is valid, meaning that it has been created and not yet destroyed
		inline bool isValid() const { return m_isValid; }

		const glm::mat3& getTransformMatrix() const;

	private: /* functions */

		// Updates the transform matrix with current position, rotation and scale.
		void updateTransformMatrix() const;

	protected: /* variables*/

		glm::vec2 m_position = glm::vec2(-1.0f, -1.0f);
		float m_rotation = -1.0f; // in radians
		glm::vec2 m_scale = glm::vec2(-1.0f, -1.0f);

		glm::vec4 m_color = glm::vec4(-1.0f, -1.0f, -1.0f, -1.0f);

		// Flag indicating if world vertices in derived class need to be updated before use
		mutable bool m_needUpdateVerticesWorld = true;

	private: /* variables */

		// Flag indicating if shape is valid, meaning that it has been created and not yet destroyed
		bool m_isValid = false;

		// A 2D transform matrix, containing position, rotation and scale,
		// used to transform vertices from local space to world space
		mutable glm::mat3 m_transformMatrix = glm::mat3(0.0f);

		// Flag indicating if transform matrix has to be updated before use
		mutable bool m_needUpdateTransformMatrix = false;
	};

} // namespace Renderer
} // namespace Renderer