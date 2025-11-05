#pragma once

#include "CameraComponent2D.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace Pekan
{
namespace Renderer2D
{

    class CameraSystem2D
    {
    public:

        // Returns the primary camera from the given registry.
		// If there are multiple primary cameras, returns the first one found.
        // If there is no primary camera, returns the default one.
		static const CameraComponent2D& getPrimaryCamera(const entt::registry& registry);

        // Returns the controllable camera from the given registry.
        // If there are multiple controllable cameras, returns the first one found.
        // If there is no controllable camera, returns null.
        static const CameraComponent2D* getControllableCamera(const entt::registry& registry);
        static CameraComponent2D* getControllableCamera(entt::registry& registry);
    };

} // namespace Renderer2D
}// namespace Pekan