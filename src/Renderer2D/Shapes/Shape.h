#pragma once

#include "Transformable2D.h"
#include "RenderCommands.h"
#include "Camera2D.h"

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

	// A vertex of a 2D shape
	struct ShapeVertex
	{
		glm::vec2 position = { 0.0f, 0.0f };

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		float shapeIndex = -1.0f;
#else
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
#endif
	};

	// A base class for 2D shapes
	class Shape : public Transformable2D
	{
	public:

		// Checks if shape is valid, meaning that it has been created and not yet destroyed
		inline bool isValid() const { return m_isValid; }

		void render() const;

		// Sets shape's color
		void setColor(glm::vec4 color);
		// Returns shape's color
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

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		// Sets shape's index inside of its batch.
		// This index will determine the value of the "shapeIndex" attribute of shape's vertices
		inline void setShapeIndex(float shapeIndex) const { m_shapeIndex = shapeIndex; }
#endif

	protected: /* functions */

		// Creates a shape.
		// @param[in] dynamic - Indicates if shape will be changed/transformed often. Used for optimization.
		//
		// NOTE: Cannot be used directly on this class.
		//       To be used by derived classes' create() function.
		void _create(bool dynamic);
		// Destroys a shape.
		//
		// NOTE: Cannot be used directly on this class.
		//       To be used by derived classes' create() function.
		void _destroy();

	protected: /* variables*/

		// Flag indicating if world vertices in derived class need to be updated before use
		mutable bool m_needUpdateVerticesWorld = true;

		glm::vec4 m_color = glm::vec4(-1.0f, -1.0f, -1.0f, -1.0f);

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
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
#endif

	private: /* functions */

		// Called by base class Transformable2D when transform changes
		void onTransformChanged() override;

	private: /* variables */

		// Flag indicating if shape is valid, meaning that it has been created and not yet destroyed
		bool m_isValid = false;

		// Flag indicating if dynamic buffers should be used for shape's vertices and indices.
		// Set to true if shape's vertices/indices are going to be changed often.
		// Used for optimization.
		bool m_isDynamic = true;
	};

} // namespace Renderer2D
} // namespace Pekan
