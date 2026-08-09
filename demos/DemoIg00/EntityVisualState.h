#pragma once

#include <glm/glm.hpp>

namespace Demo
{

	enum class EntityAnim
	{
		Idle,
		Run,
		Jump,
		Attack
	};

	// Snapshot of an entity for rendering — no GPU / draw details.
	// Renderers decide how to visualize this (direct texture, dots, etc.).
	struct EntityVisualState
	{
		glm::vec2 position = { 0.0f, 0.0f };
		glm::vec2 size = { 1.0f, 1.0f }; // bounding box width / height
		bool facingRight = true;
		EntityAnim anim = EntityAnim::Idle;
		int frame = 0;
		// 1 = fully visible player texture/effect, 0 = fully replaced by glow
		float opacity = 1.0f;
	};

} // namespace Demo
