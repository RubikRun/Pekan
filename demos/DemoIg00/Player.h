#pragma once

#include "EntityVisualState.h"

namespace Demo
{

	class Player
	{
	public:
		void create(glm::vec2 position, glm::vec2 size);
		void update(float dt, float groundTopY);
		EntityVisualState getVisualState() const;

		glm::vec2 getPosition() const { return m_position; }
		glm::vec2 getSize() const { return m_size; }

	private:
		void updateAnimation(float dt);

		glm::vec2 m_position = { 0.0f, 0.0f };
		glm::vec2 m_size = { 1.0f, 1.0f };
		glm::vec2 m_velocity = { 0.0f, 0.0f };

		bool m_facingRight = true;
		bool m_grounded = false;

		EntityAnim m_anim = EntityAnim::Idle;
		int m_frame = 0;
		float m_animTimer = 0.0f;
		bool m_attacking = false;
		bool m_attackKeyWasDown = false;
	};

} // namespace Demo
