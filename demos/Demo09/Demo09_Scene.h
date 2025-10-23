#pragma once

#include "Scene.h"

namespace Demo
{

	class Demo09_Scene : public Pekan::Scene
	{
	public:

		Demo09_Scene(Pekan::PekanApplication* application) : Pekan::Scene(application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() const override;

		void exit() override;

	private:

		entt::entity m_turkey = entt::null;
	};

} // namespace Demo
