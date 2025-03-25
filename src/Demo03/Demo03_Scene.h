#pragma once

#include "PekanScene.h"

#include "Snake.h"
#include "RenderObject.h"

namespace Demo
{

	class Demo03_Scene : public Pekan::PekanScene
	{
	public:

		bool init() override;

		void update() override;

		void render() override;

		void exit() override;

	private: /* variables */

		Snake m_snake;

		RenderObject m_apple;

		glm::vec2 m_applePosition = { 0.0f, 0.0f };

		float m_aspectRatio = 0.0f;
	};

} // namespace Demo