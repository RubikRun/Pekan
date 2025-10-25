#pragma once

#include "Scene.h"

namespace Pekan
{
namespace Renderer2D {

	class Scene2D : public Scene
	{
	public: /* functions */

		Scene2D(PekanApplication* application) : Scene(application) {}
		void render() const override;

	protected: /* functions */

		// Can be implemented by derived classes with specific render logic
		virtual void _render() const {}
	};

} // namespace Renderer2D
} // namespace Pekan