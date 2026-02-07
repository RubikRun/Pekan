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

		entt::registry& getRegistry() { return m_registry; }
		const entt::registry& getRegistry() const { return m_registry; }

		// Creates a new entity in the scene and returns it.
		entt::entity createEntity();
		// Destroys the given entity in the scene.
		void destroyEntity(entt::entity entity);

		// Enables the given entity in the scene.
		void enableEntity(entt::entity entity);
		// Disables the given entity in the scene.
		void disableEntity(entt::entity entity);

	private: /* variables */

		entt::registry m_registry;
	};

} // namespace Pekan