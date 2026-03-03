#pragma once

#include "Layer.h"

#include "Snake.h"
#include "Apple.h"

namespace Demo
{

	class Demo03_Scene : public Pekan::Layer
	{
		bool init() override;
		void exit() override;
		void update(double deltaTime) override;
		void render() const override;

	public:

		Demo03_Scene(Pekan::PekanApplication* application) : Layer(application) {}

		std::string getLayerName() const override { return "scene_layer"; }

	private: /* functions */

		bool onKeyPressed(const Pekan::KeyPressedEvent& event) override;

	private: /* variables */

		Snake m_snake;

		Apple m_apple;
	};

} // namespace Demo