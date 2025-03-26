#pragma once

#include "PekanScene.h"

#include "Snake.h"
#include "Apple.h"

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

		Apple m_apple;
	};

} // namespace Demo