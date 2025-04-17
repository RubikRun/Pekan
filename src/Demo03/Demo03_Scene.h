#pragma once

#include "Layer.h"

#include "Snake.h"
#include "Apple.h"

namespace Demo
{

	class Demo03_Scene : public Pekan::Layer
	{
	public:

		Demo03_Scene(Pekan::PekanApplication* application) : Layer("scene_layer", application) {}

		bool init() override;

		void update() override;

		void render() override;

		void exit() override;

	private: /* functions */

		bool onKeyPressed(Pekan::KeyPressedEvent& event) override;

	private: /* variables */

		Snake m_snake;

		Apple m_apple;
	};

} // namespace Demo