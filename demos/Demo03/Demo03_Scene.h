#pragma once

#include "Layer.h"

#include "Snake.h"
#include "Apple.h"

namespace Demo
{

	class Demo03_Scene : public Pekan::Layer
	{
	public:

		Demo03_Scene(Pekan::PekanApplication* application) : Layer(application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() const override;

		void exit() override;

		std::string getLayerName() const override { return "scene_layer"; }

	private: /* functions */

		bool onKeyPressed(const Pekan::KeyPressedEvent& event) override;

	private: /* variables */

		Snake m_snake;

		Apple m_apple;
	};

} // namespace Demo