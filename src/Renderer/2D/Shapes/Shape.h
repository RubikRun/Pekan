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
		float shapeIndex = -1.0f;
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

		// Checks if shape is dynamic,
		// meaning it will be changed/transformed often,
		// and it's better to use dynamic buffers for its vertices and indices.
		inline bool isDynamic() const { return m_isDynamic; }

		// Sets shape's index inside of its batch.
		// This index will determine the value of the "shapeIndex" attribute of shape's vertices
		inline void setShapeIndex(float shapeIndex) const { m_shapeIndex = shapeIndex; }

	protected: /* functions */

		// @param[in] dynamic - Indicates if shape will be changed/transformed often. Used for optimization.
		void create(bool dynamic);
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

		// Shape's index inside of its batch.
		// To be used by derived classes to set each vertex's "shapeIndex" attribute.
		//
		// NOTE: Marked as "mutable" because it doesn't reflect a shape's state exactly.
		// It reflects the state of "a shape inside a batch".
		// We want to be able to change a shape's index in a "const" way
		// because it doesn't change the actual shape.
		// This can, of course, be avoided by letting a Shape be "just a shape"
		// and return only its actual vertices without any batch-related metadata,
		// but it would be inefficient, because then a batch will have to copy all vertices
		// in order to add the "shapeIndex" attribute to them.
		mutable float m_shapeIndex = -1.0f;

	private: /* variables */

		// Flag indicating if shape is valid, meaning that it has been created and not yet destroyed
		bool m_isValid = false;

		// Flag indicating if dynamic buffers should be used for shape's vertices and indices.
		// Set to true if shape's vertices/indices are going to be changed often.
		// Used for optimization.
		bool m_isDynamic = true;

		// A 2D transform matrix, containing position, rotation and scale,
		// used to transform vertices from local space to world space
		mutable glm::mat3 m_transformMatrix = glm::mat3(0.0f);

		// Flag indicating if transform matrix has to be updated before use
		mutable bool m_needUpdateTransformMatrix = false;
	};

} // namespace Renderer
} // namespace Pekan
