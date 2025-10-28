#include "Scene2D.h"

#include "Renderer2DSystem_ECS.h"
#include "RenderCommands.h"
#include "RenderState.h"

using namespace Pekan::Graphics;

namespace Pekan
{
namespace Renderer2D {

	void Scene2D::render() const
	{
		// TEMP: remove after TO-DO item 0071 is done
		RenderCommands::clear();

		const entt::registry& registry = getRegistry();
		Renderer2DSystem_ECS::render(registry);

		_render();
	}

} // namespace Renderer2D
} // namespace Pekan