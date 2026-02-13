#pragma once

#include "Scene2D.h"
#include <entt/entt.hpp>
#include <string>
#include <vector>

namespace Pekan
{
namespace Editor
{

	class EditorScene : public Pekan::Renderer2D::Scene2D
	{
	public:

		EditorScene(Pekan::PekanApplication* application) : Pekan::Renderer2D::Scene2D(application) {}

		// Adds a new entity to the scene
		void addEntity();

		// Returns a list of all entities in the scene
		const std::vector<entt::entity>& getEntities() const { return m_entities; }

		// Adds a component of the given type to a given entity
		// @param[in] entity - the entity to which the component will be added
		// @param[in] componentTypeIndex - the index of the component type to add
		void addComponent(entt::entity entity, int componentTypeIndex);

		// Returns a list of names of component types supported in the editor
		static const std::vector<std::string>& getComponentTypesNames();

	private: /* functions */

		bool _init() override;
		void _exit() override;

		void update(double deltaTime) override;

	private: /* variables */

		// List of all entities in the scene
		std::vector<entt::entity> m_entities;

		// Scene's underlying ECS registry
		entt::registry& m_registry = getRegistry();
	};

} // namespace Editor
} // namespace Pekan
