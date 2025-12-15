#include "CameraSystem2D.h"

#include "CameraComponent2D.h"
#include "PekanEngine.h"
#include "PekanLogger.h"

namespace Pekan
{
namespace Renderer2D
{

    // A default CameraComponent2D instance ([-1, 1] range in X and Y) to use if there is no primary camera.
    static constexpr CameraComponent2D g_defaultCamera =
    {
        .size = { 2.0f, 2.0f },
        .position = { 0.0f, 0.0f },
        .zoomLevel = 1.0f
    };

    ////////////////////////////////////////////////////////////////////////////////////////
    // CameraSystem2D implementation
    ////////////////////////////////////////////////////////////////////////////////////////

    const CameraComponent2D& CameraSystem2D::getPrimaryCamera(const entt::registry& registry)
    {
        const auto view = registry.view<CameraComponent2D>();
        for (const entt::entity entity : view)
        {
            const CameraComponent2D& camera = view.get<CameraComponent2D>(entity);
            if (camera.isPrimary)
            {
                return camera;
            }
        }
        return g_defaultCamera;
    }

    const CameraComponent2D* CameraSystem2D::getControllableCamera(const entt::registry& registry)
    {
        const auto view = registry.view<CameraComponent2D>();
        for (const entt::entity entity : view)
        {
            const CameraComponent2D& camera = view.get<CameraComponent2D>(entity);
            if (camera.isControllable)
            {
                return &camera;
            }
        }
        return nullptr;
    }

    CameraComponent2D* CameraSystem2D::getControllableCamera(entt::registry& registry)
    {
        const auto view = registry.view<CameraComponent2D>();
        for (const entt::entity entity : view)
        {
            CameraComponent2D& camera = view.get<CameraComponent2D>(entity);
            if (camera.isControllable)
            {
                return &camera;
            }
        }
        return nullptr;
    }

} // namespace Renderer2D
} // namespace Pekan