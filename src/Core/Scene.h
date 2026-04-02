#pragma once

#include "Layer.h"

#include <entt/entt.hpp>

namespace Pekan
{

	class Scene : public Layer
	{
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
		entt::entity createEntity();
		// Destroys the given entity in the scene.
		void destroyEntity(entt::entity entity);

		// Enables the given entity in the scene.
		void enableEntity(entt::entity entity);
		// Disables the given entity in the scene.
		void disableEntity(entt::entity entity);

	private: /* variables */

		// Scene's underlying ECS registry
		// containing all entities and their components
		entt::registry m_registry;

		// List of all entities in the scene.
		// More precisely, this is a list of entity IDs.
		// The actual entities live in the registry.
		std::vector<entt::entity> m_entities;
	};

} // namespace Pekan