#pragma once

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

	// A base class for 2D transformable objects
	class Transformable2D
	{
	public:

		// Sets a new position to the object
		void setPosition(glm::vec2 position);
		// Sets a new rotation to the object
		void setRotation(float rotation); // in radians
		// Sets a new scale to the object
		void setScale(glm::vec2 scale);

		// Moves the object, changing its position by some amount
		void move(glm::vec2 deltaPosition);
		// Rotates the object, changing its rotation by some amount
		void rotate(float deltaRotation);
		// Scales the object, changing its scale by some amount
		void scale(glm::vec2 deltaScale);

		// Returns object's position
		inline glm::vec2 getPosition() const { return m_position; }
		// Returns object's rotation
		inline float getRotation() const { return m_rotation; }
		// Returns object's scale
		inline glm::vec2 getScale() const { return m_scale; }

		// Returns object's transform matrix containing the combined action
		// of transforming it with its position, rotation and scale
		const glm::mat3& getTransformMatrix() const;

	protected:

		// Creates a 2D transformable object.
		//
		// NOTE: Cannot be used directly on this class.
		//       To be used by derived classes' create() function.
		void _create();
		// Destroys a 2D transformable object.
		//
		// NOTE: Cannot be used directly on this class.
		//       To be used by derived classes' create() function.
		void _destroy();

	private: /* functions */

		// Updates the transform matrix with current position, rotation and scale.
		void updateTransformMatrix() const;

		// Can be implemented by derived classes to react to transform changes.
		// Whenever position, rotation or scale changes this function will be called.
		virtual void onTransformChanged() {}

	private: /* variables*/

		// Position, rotation and scale of the transformable object,
		// used to transform the object from local space to world space
		glm::vec2 m_position = glm::vec2(-1.0f, -1.0f);
		float m_rotation = -1.0f; // in radians
		glm::vec2 m_scale = glm::vec2(-1.0f, -1.0f);

		// A 2D transform matrix, containing the combined action
		// of transforming the object with its position, rotation and scale.
		mutable glm::mat3 m_transformMatrix = glm::mat3(1.0f);

		// Flag indicating if transform matrix has to be updated before use
		mutable bool m_needUpdateTransformMatrix = false;
	};

} // namespace Renderer2D
} // namespace Pekan
