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

	protected: /* functions */

		entt::entity createEntity();
		void destroyEntity(entt::entity entity);

	private: /* variables */

		entt::registry m_registry;
	};

} // namespace Pekan