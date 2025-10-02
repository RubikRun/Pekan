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

		// Sets a given transformable object to be the parent of this transformable object
		void setParent(const Transformable2D* parent);

		// Sets object's local position
		void setPosition(glm::vec2 position);
		// Sets object's local rotation
		void setRotation(float rotation); // in radians
		// Sets object's local scale
		void setScale(glm::vec2 scale);

		// Moves object by some amount, changing its local position
		void move(glm::vec2 deltaPosition);
		// Rotates object by some amount, changing its local rotation
		void rotate(float deltaRotation);
		// Scales object by some amount, changing its local scale
		void scale(glm::vec2 deltaScale);

		// Returns object's local position
		glm::vec2 getPosition() const { return m_position; }
		// Returns object's world position
		glm::vec2 getPositionInWorld() const;
		// Returns object's local rotation
		float getRotation() const { return m_rotation; }
		// Returns object's world rotation
		float getRotationInWorld() const;
		// Returns object's local scale
		glm::vec2 getScale() const { return m_scale; }
		// Returns object's world scale
		glm::vec2 getScaleInWorld() const;

		// Returns object's local matrix.
		// An object's local matrix is a 3x3 matrix
		// that transforms 2D points from object's local space to parent's local space
		// (or to world space, if there is no parent)
		const glm::mat3& getLocalMatrix() const;
		// Returns object's world matrix.
		// An object's world matrix is a 3x3 matrix
		// that transforms 2D points from object's local space to world space
		const glm::mat3& getWorldMatrix() const;

		// Returns object's change ID,
		// uniquely identifying the current transform state of the object.
		//
		// Users of this class can be sure that if an object's change ID
		// has changed since they last called getChangeId(),
		// then object's world matrix has also changed, and vice versa.
		//
		// NOTE: A change ID is unique only in the scope of a single object.
		//       DO NOT compare the change ID of one object with the change ID of another object.
		//       obj1.getChangeId() == obj2.getChangeId() DOES NOT mean the two objects have the same transform.
		unsigned getChangeId() const;

	protected:

		// Creates/destroys transformable object.
		// To be called by derived classes in their create()/destroy() functions.
		void _create();
		void _destroy();

	private: /* functions */

		// Updates cached local matrix with current position, rotation and scale, if it's dirty
		void updateLocalMatrix() const;
		// Updates cached world matrix with current parent and current position, rotation and scale, if it's dirty
		void updateWorldMatrix() const;

		// Registers a change done to the local state (position, rotation and scale) of the object.
		// To be called whenever local state changes.
		void registerLocalChange() const;
		// Registers a change done to the parent of the object.
		// To be called whenever parent changes.
		void registerParentChange() const;

	private: /* variables */

		// Object's local position
		glm::vec2 m_position = glm::vec2(-1.0f, -1.0f);
		// Object's local rotation, in radians
		float m_rotation = -1.0f;
		// Object's local scale
		glm::vec2 m_scale = glm::vec2(-1.0f, -1.0f);

		// Object's parent.
		//
		// A transformable object is transformed relative to its parent,
		// so a child will begin at wherever the parent is in world space,
		// and then it will be transformed additionally with its own local transform
		const Transformable2D* m_parent = nullptr;

		// Cached local matrix.
		//
		// The local matrix is a 3x3 matrix that transforms 2D points
		// from object's local space to parent's local space.
		// More specifically, points will be transformed with object's local transform (position, rotation and scale) ignoring the parent.
		mutable glm::mat3 m_localMatrix = glm::mat3(1.0f);

		// Cached world matrix.
		//
		// The world matrix is a 3x3 matrix that transforms 2D points
		// from object's local space to world space.
		// More specifically, points will be transformed with object's local transform (position, rotation and scale)
		// and then parent's local transform will be applied on top,
		// recursively applying parent's parent's local transform, etc.
		mutable glm::mat3 m_worldMatrix = glm::mat3(1.0f);

		// A flag indicating if the cached local matrix is "dirty"
		// which means that it needs to be updated
		mutable bool m_isDirtyLocalMatrix = true;
		// A flag indicating if the cached world matrix is "dirty"
		// which means that it needs to be updated
		mutable bool m_isDirtyWorldMatrix = true;

		// Object's change ID,
		// uniquely identifying the current transform state of the object.
		// If anything about the object changes (including its parent) the change ID will be different.
		// Users of this class can use the change ID as a quick and easy way to check if an object has changed its transform.
		mutable unsigned m_changeId = 0;

		// Cached change ID of parent.
		// This is the parent's change ID "used" in this object's current change ID.
		// Parent's actual current change ID might be different, because parent might have changed without this object noticing.
		// When this object notices, it will change its own change ID to account for parent's change.
		mutable unsigned m_cachedParentChangeId = 0;

		// Change ID of parent used when calculating the currently cached world matrix.
		mutable unsigned m_parentChangeIdUsedInWorldMatrix = 0;
	};

} // namespace Renderer2D
} // namespace Pekan
