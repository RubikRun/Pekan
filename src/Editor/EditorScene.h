#pragma once

#include "Scene2D.h"
#include <entt/entt.hpp>
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

		// Returns a vector of all entities in the scene
		const std::vector<entt::entity>& getEntities() const { return m_entities; }

	private: /* functions */

		bool _init() override;
		void _exit() override;

		void update(double deltaTime) override;

	private: /* variables */

		std::vector<entt::entity> m_entities;
	};

} // namespace Editor
} // namespace Pekan
