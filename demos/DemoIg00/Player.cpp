#include "Player.h"

#include "PekanEngine.h"
#include "Events/KeyEvents.h"

#include <cmath>

using Pekan::PekanEngine;
using Pekan::KeyCode;

namespace Demo
{

	static constexpr float MOVE_SPEED = 6.0f;
	static constexpr float JUMP_SPEED = 12.0f;
	static constexpr float GRAVITY = 28.0f;
	static constexpr float ANIM_FPS = 12.0f;
	static constexpr int ANIM_FRAME_COUNT = 10;

	void Player::create(glm::vec2 position, glm::vec2 size)
	{
		m_position = position;
		m_size = size;
		m_velocity = { 0.0f, 0.0f };
		m_facingRight = true;
		m_grounded = false;
		m_anim = EntityAnim::Idle;
		m_frame = 0;
		m_animTimer = 0.0f;
	}

	void Player::update(float dt, float groundTopY)
	{
		float moveDir = 0.0f;
		if (PekanEngine::isKeyPressed(KeyCode::KEY_A) || PekanEngine::isKeyPressed(KeyCode::KEY_LEFT))
		{
			moveDir -= 1.0f;
		}
		if (PekanEngine::isKeyPressed(KeyCode::KEY_D) || PekanEngine::isKeyPressed(KeyCode::KEY_RIGHT))
		{
			moveDir += 1.0f;
		}

		m_velocity.x = moveDir * MOVE_SPEED;
		if (moveDir > 0.0f) m_facingRight = true;
		else if (moveDir < 0.0f) m_facingRight = false;

		const bool jumpPressed = PekanEngine::isKeyPressed(KeyCode::KEY_SPACE);

		if (jumpPressed && m_grounded)
		{
			m_velocity.y = JUMP_SPEED;
			m_grounded = false;
		}

		m_velocity.y -= GRAVITY * dt;
		m_position += m_velocity * dt;

		const float feetY = m_position.y - m_size.y * 0.5f;
		if (feetY <= groundTopY && m_velocity.y <= 0.0f)
		{
			m_position.y = groundTopY + m_size.y * 0.5f;
			m_velocity.y = 0.0f;
			m_grounded = true;
		}
		else
		{
			m_grounded = false;
		}

		updateAnimation(dt);
	}

	void Player::updateAnimation(float dt)
	{
		EntityAnim desired = EntityAnim::Idle;
		if (!m_grounded)
		{
			desired = EntityAnim::Jump;
		}
		else if (std::abs(m_velocity.x) > 0.01f)
		{
			desired = EntityAnim::Run;
		}

		if (desired != m_anim)
		{
			m_anim = desired;
			m_frame = 0;
			m_animTimer = 0.0f;
		}

		m_animTimer += dt;
		const float frameDuration = 1.0f / ANIM_FPS;
		while (m_animTimer >= frameDuration)
		{
			m_animTimer -= frameDuration;
			if (m_anim == EntityAnim::Jump)
			{
				if (m_frame < ANIM_FRAME_COUNT - 1)
				{
					m_frame++;
				}
			}
			else
			{
				m_frame = (m_frame + 1) % ANIM_FRAME_COUNT;
			}
		}
	}

	EntityVisualState Player::getVisualState() const
	{
		EntityVisualState state;
		state.position = m_position;
		state.size = m_size;
		state.facingRight = m_facingRight;
		state.anim = m_anim;
		state.frame = m_frame;
		return state;
	}

} // namespace Demo
