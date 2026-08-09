#include "DirectSpriteRenderer.h"

#include "Image.h"
#include "PekanLogger.h"

#include <string>

using namespace Pekan::Graphics;
using namespace Pekan::Renderer2D;

namespace Demo
{

	static constexpr int FRAMES_PER_ANIM = 10;
	static constexpr unsigned char ALPHA_THRESHOLD = 16;

	// Image pixels are flipped vertically on load (OpenGL), so row 0 is the visual bottom.
	static float computeBottomPadFraction(const Image& image)
	{
		if (!image.isValid() || image.getWidth() <= 0 || image.getHeight() <= 0)
		{
			return 0.0f;
		}

		const int channels = image.getNumChannels();
		if (channels < 4)
		{
			return 0.0f;
		}

		const int w = image.getWidth();
		const int h = image.getHeight();
		const unsigned char* data = image.getData();

		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				if (data[(y * w + x) * channels + 3] > ALPHA_THRESHOLD)
				{
					return static_cast<float>(y) / static_cast<float>(h);
				}
			}
		}
		return 0.0f;
	}

	static bool loadAnim(
		const char* prefix,
		std::vector<Texture2D_Ptr>& textures,
		std::vector<float>& bottomPadFractions,
		float& outAspect)
	{
		textures.clear();
		bottomPadFractions.clear();
		textures.resize(FRAMES_PER_ANIM);
		bottomPadFractions.resize(FRAMES_PER_ANIM, 0.0f);
		outAspect = 1.0f;

		for (int i = 0; i < FRAMES_PER_ANIM; i++)
		{
			std::string path = "resources/NinjaSprites/";
			path += prefix;
			path += "__";
			if (i < 10)
			{
				path += "00";
			}
			else if (i < 100)
			{
				path += "0";
			}
			path += std::to_string(i);
			path += ".png";

			Image image(path.c_str());
			if (!image.isValid())
			{
				PK_LOG_ERROR(("Failed to load texture: " + path).c_str(), "DemoIg00");
				return false;
			}

			if (i == 0 && image.getHeight() > 0)
			{
				outAspect = static_cast<float>(image.getWidth()) / static_cast<float>(image.getHeight());
			}

			bottomPadFractions[i] = computeBottomPadFraction(image);

			textures[i] = std::make_shared<Texture2D>();
			textures[i]->create(image);
		}
		return true;
	}

	bool DirectSpriteRenderer::init()
	{
		if (!loadAnim("Idle", m_idle.textures, m_idle.bottomPadFractions, m_idle.aspect)) return false;
		if (!loadAnim("Run", m_run.textures, m_run.bottomPadFractions, m_run.aspect)) return false;
		if (!loadAnim("Jump", m_jump.textures, m_jump.bottomPadFractions, m_jump.aspect)) return false;
		if (!loadAnim("Attack", m_attack.textures, m_attack.bottomPadFractions, m_attack.aspect)) return false;
		if (!loadAnim("Throw", m_throw.textures, m_throw.bottomPadFractions, m_throw.aspect)) return false;

		m_sprite.create(m_idle.textures[0], 1.0f, 1.0f);
		return true;
	}

	const DirectSpriteRenderer::AnimData& DirectSpriteRenderer::getAnimData(EntityAnim anim) const
	{
		switch (anim)
		{
		case EntityAnim::Idle: return m_idle;
		case EntityAnim::Run:  return m_run;
		case EntityAnim::Jump: return m_jump;
		case EntityAnim::Attack: return m_attack;
		case EntityAnim::Throw: return m_throw;
		}
		return m_idle;
	}

	int DirectSpriteRenderer::clampFrame(const AnimData& anim, int frame) const
	{
		if (frame < 0) return 0;
		if (frame >= static_cast<int>(anim.textures.size())) return static_cast<int>(anim.textures.size()) - 1;
		return frame;
	}

	void DirectSpriteRenderer::render(const EntityVisualState& state) const
	{
		const AnimData& anim = getAnimData(state.anim);
		const int frame = clampFrame(anim, state.frame);
		if (anim.textures.empty() || !m_sprite.isValid())
		{
			return;
		}

		const Texture2D_ConstPtr& texture = anim.textures[frame];
		if (texture == nullptr)
		{
			return;
		}

		const float height = state.size.y;
		const float width = height * anim.aspect;
		const float bottomPad = anim.bottomPadFractions[frame];

		// Shift down so opaque feet sit on the entity bbox bottom (Idle has ~0 pad;
		// Run/Jump frames have varying empty rows under the feet).
		glm::vec2 drawPos = state.position;
		drawPos.y -= bottomPad * height;

		// Manual nudge: ~5 screen pixels lower (camera height 10, window 720).
		constexpr float extraFootOffsetWorld = 5.0f * 10.0f / 720.0f;
		drawPos.y -= extraFootOffsetWorld;

		m_sprite.setTexture(texture);
		m_sprite.setWidth(width);
		m_sprite.setHeight(height);
		m_sprite.setPosition(drawPos);
		m_sprite.setScale({ state.facingRight ? 1.0f : -1.0f, 1.0f });
		m_sprite.render();
	}

	void DirectSpriteRenderer::destroy()
	{
		if (m_sprite.isValid())
		{
			m_sprite.destroy();
		}

		auto destroyAnim = [](AnimData& anim)
		{
			for (auto& t : anim.textures) { if (t) t->destroy(); }
			anim.textures.clear();
			anim.bottomPadFractions.clear();
		};
		destroyAnim(m_idle);
		destroyAnim(m_run);
		destroyAnim(m_jump);
	}

} // namespace Demo
