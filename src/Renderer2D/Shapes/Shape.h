#pragma once

#include "Transformable2D.h"
#include "RenderCommands.h"
#include "Vertex2D.h"

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

	// A base class for 2D shapes
	class Shape : public Transformable2D
	{
	public:

		void render() const;

		// Returns shape's color
		glm::vec4 getColor() const { return m_color; }
		// Sets shape's color
		void setColor(glm::vec4 color);

#if PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
		// To be implemented by derived classes to return their vertex data in world space.
		// @param[in] shapeIndex - Shape's index inside of its batch. Determines the value of the "shapeIndex" attribute of shape's vertices
		virtual const Vertex2D* getVertices(float shapeIndex) const = 0;
#else
		// To be implemented by derived classes to return their vertex data in world space.
		virtual const Vertex2D* getVertices() const = 0;
#endif
		// To be implemented by derived classes to return the number of their vertices.
		virtual int getVerticesCount() const = 0;

		// Can be overriden by derived classes to return their index data, if indices are used at all.
		virtual const unsigned* getIndices() const { return nullptr; }
		// Can be overriden by derived classes to return the number of their indices, if indices are used at all.
		virtual int getIndicesCount() const { return 0; }

		// Checks if shape is valid, meaning that it has been created and not yet destroyed
		bool isValid() const { return m_isValid; }

	protected: /* functions */

		// Creates a shape.
		//
		// NOTE: Cannot be used directly on this class.
		//       To be used by derived classes' create() function.
		void _create();
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
	};

} // namespace Renderer2D
} // namespace Pekan
