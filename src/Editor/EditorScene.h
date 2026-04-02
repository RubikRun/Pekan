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
		bool _init() override;
		void _exit() override;

		void update(double deltaTime) override;

	public:

		EditorScene(Pekan::PekanApplication* application) : Pekan::Renderer2D::Scene2D(application) {}

		// Adds a new entity to the scene
		void addEntity();

		// Enables the given entity in the scene
		void enableEntity(entt::entity entity);
		// Disables the given entity in the scene
		void disableEntity(entt::entity entity);

		// Adds a component of the given type to a given entity
		// @param[in] entity - the entity to which the component will be added
		// @param[in] componentTypeIndex - the index of the component type to add
		void addComponent(entt::entity entity, int componentTypeIndex);

		// Returns a list of names of component types supported in the editor
		static const std::vector<std::string>& getComponentTypesNames();

	private: /* variables */

		// Scene's underlying ECS registry
		entt::registry& m_registry = getRegistry();
	};

} // namespace Editor
} // namespace Pekan
