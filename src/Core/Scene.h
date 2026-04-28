#pragma once

#include "Layer.h"
#include "Entity/EntityID.h"

#include <entt/entt.hpp>
#include <vector>

namespace Pekan
{
	class SceneSerializer;

	class Scene : public Layer
	{
		// SceneSerializer is responsible for save/load of a scene.
		// It needs to call non-public scene-management methods,
		// for example: createEntity(EntityID), setNextEntityId(EntityID)
		friend class SceneSerializer;

	public: /* functions */

		Scene(PekanApplication* application) : Layer(application) {}

		// Can be implemented by derived classes
		// if they want a non-default name for their scene's layer
		virtual std::string getLayerName() const override { return "scene_layer"; }

		// Returns the unerlying ECS registry
		entt::registry& getRegistry() { return m_registry; }
		const entt::registry& getRegistry() const { return m_registry; }

		// Returns a list of all entities in the scene
		const std::vector<entt::entity>& getEntities() const { return m_entities; }

	protected: /* functions */

		// Creates a new entity in the scene and returns it.
		// Attaches an EntityIDComponent with a newly-generated, unique EntityID.
		entt::entity createEntity();
		// Destroys the given entity in the scene.
		void destroyEntity(entt::entity entity);

		// Enables the given entity in the scene.
		void enableEntity(entt::entity entity);
		// Disables the given entity in the scene.
		void disableEntity(entt::entity entity);

		// Wipes the scene, destroying all entities and components in the registry,
		// clearing the entity list, and resetting the "next EntityID" counter.
		void clear();

		// Moves / takes ownership of another scene's state,
		// including its registry, entity list, and "next EntityID" counter.
		// Other scene's state (layer-level state, application pointer, etc.)
		// is NOT touched - only the data that defines the scene's contents is moved.
		void adoptFrom(Scene&& other);

	private: /* functions */

		// Creates a new entity in the scene with the given EntityID.
		//
		// Does NOT advance the "next EntityID" counter - the caller is responsible for
		// calling setNextEntityId() after finished creating entities via this method.
		entt::entity createEntity(EntityID entityId);

		// Sets the "next EntityID" counter - the value of this counter
		// determines the next EntityID that will be assigned by createEntity().
		//
		// WARNING: EntityIDs are assigned incrementally, so the value provided here must be such that:
		// - it's a valid non-used EntityID, and
		// - all greater EntityIDs are also valid and unused
		void setNextEntityId(EntityID id);

	private: /* variables */

		// Scene's underlying ECS registry
		// containing all entities and their components
		entt::registry m_registry;

		// List of all entities in the scene.
		// More precisely, this is a list of entity IDs.
		// The actual entities live in the registry.
		std::vector<entt::entity> m_entities;

		// The value of this counter determines the next EntityID that will be assigned by createEntity().
		// It is incremented after each successful call to createEntity().
		// Always >= 1, since 0 is reservewd for INVALID_ENTITY_ID.
		EntityID m_nextEntityId = 1;
	};

} // namespace Pekan
