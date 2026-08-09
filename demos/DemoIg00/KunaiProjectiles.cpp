#include "KunaiProjectiles.h"

#include "Image.h"
#include "PekanLogger.h"
#include "Renderer2DSystem.h"

#include <algorithm>
#include <cmath>

using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;

namespace Demo
{

	static constexpr float KUNAI_SPEED = 7.5f;

	bool KunaiProjectiles::init()
	{
		Image image("resources/NinjaSprites/Kunai.png");
		if (!image.isValid())
		{
			PK_LOG_ERROR("Failed to load Kunai.png", "DemoIg00");
			return false;
		}

		m_texture = std::make_shared<Texture2D>();
		m_texture->create(image);

		const float aspect = (image.getHeight() > 0)
			? static_cast<float>(image.getWidth()) / static_cast<float>(image.getHeight())
			: 2.0f;
		m_kunaiHeight = 0.65f;
		m_kunaiWidth = m_kunaiHeight * aspect;

		m_sprite.create(m_texture, m_kunaiWidth, m_kunaiHeight);
		m_kunais.clear();
		return true;
	}

	void KunaiProjectiles::spawn(glm::vec2 playerPos, glm::vec2 playerSize, bool facingRight)
	{
		Kunai k;
		const float dir = facingRight ? 1.0f : -1.0f;
		k.position = {
			playerPos.x - dir * playerSize.x * 0.15f,
			playerPos.y - playerSize.y * 0.02f
		};
		k.velocityX = dir * KUNAI_SPEED;
		m_kunais.push_back(k);
	}

	void KunaiProjectiles::update(float dt, const Camera2D_ConstPtr& camera)
	{
		for (Kunai& k : m_kunais)
		{
			k.position.x += k.velocityX * dt;
		}

		if (camera == nullptr || m_kunais.empty())
		{
			return;
		}

		const glm::vec2 camPos = camera->getPosition();
		const glm::vec2 camSize = camera->getSize() / std::max(camera->getZoom(), 0.001f);
		const float halfW = camSize.x * 0.5f;
		const float halfH = camSize.y * 0.5f;
		constexpr float margin = 0.75f;
		const float left = camPos.x - halfW - margin;
		const float right = camPos.x + halfW + margin;
		const float bottom = camPos.y - halfH - margin;
		const float top = camPos.y + halfH + margin;

		m_kunais.erase(
			std::remove_if(m_kunais.begin(), m_kunais.end(),
				[&](const Kunai& k)
				{
					return k.position.x < left || k.position.x > right ||
						k.position.y < bottom || k.position.y > top;
				}),
			m_kunais.end()
		);
	}

	void KunaiProjectiles::render() const
	{
		if (!m_sprite.isValid() || m_kunais.empty())
		{
			return;
		}

		Renderer2DSystem::beginFrame();
		constexpr float PI = 3.14159265f;
		for (const Kunai& k : m_kunais)
		{
			// Texture points up; rotate so tip faces travel direction
			m_sprite.setPosition(k.position);
			m_sprite.setRotation(k.velocityX >= 0.0f ? PI * 0.5f : -PI * 0.5f);
			m_sprite.setScale({ 1.0f, 1.0f });
			m_sprite.render();
		}
		Renderer2DSystem::endFrame();
	}

	void KunaiProjectiles::destroy()
	{
		m_kunais.clear();
		if (m_sprite.isValid())
		{
			m_sprite.destroy();
		}
		if (m_texture != nullptr)
		{
			m_texture->destroy();
			m_texture.reset();
		}
	}

} // namespace Demo
