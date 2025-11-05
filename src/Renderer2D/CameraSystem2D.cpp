#include "CameraSystem2D.h"

#include "CameraComponent2D.h"
#include "PekanEngine.h"
#include "PekanLogger.h"

namespace Pekan
{
namespace Renderer2D
{

    // Returns a default CameraComponent2D instance
    static constexpr CameraComponent2D getDefaultCamera()
    {
        CameraComponent2D camera;
        camera.size = { 2.0f, 2.0f };
        camera.position = { 0.0f, 0.0f };
        camera.zoomLevel = 1.0f;
        return camera;
    }

    // A default CameraComponent2D instance to use if there is no primary camera
    static constexpr CameraComponent2D g_defaultCamera = getDefaultCamera();




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
        PK_LOG_WARNING("No primary CameraComponent2D camera found. A default one will be used.", "Pekan");
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