#pragma once

#include "Scene2D.h"

namespace Pekan
{
namespace Editor
{

	class EditorScene : public Pekan::Renderer2D::Scene2D
	{
	public:

		EditorScene(Pekan::PekanApplication* application) : Pekan::Renderer2D::Scene2D(application) {}

	private: /* functions */

		bool _init() override;
		void _exit() override;

		void update(double deltaTime) override;
	};

} // namespace Editor
} // namespace Pekan
