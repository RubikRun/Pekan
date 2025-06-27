#pragma once

#include "Shape.h"
#include "RenderObject.h"

namespace Pekan
{
namespace Renderer
{

    class Renderer2D
    {
        using CameraSharedPtr = std::shared_ptr<Renderer::Camera2D>;
        using CameraWeakPtr = std::weak_ptr<Renderer::Camera2D>;

    public:

        static void beginFrame();
        static void endFrame();

        static void render(const Shape& shape);

        // Sets a camera to be used for rendering
        static inline void setCamera(const CameraSharedPtr& camera) { s_camera = camera; }

    private:

        static RenderObject s_batch;

        // A pointer to the camera used for rendering.
        // NOTE: It's a weak pointer so the camera is NOT owned by Renderer2D.
        //       If the camera is destroyed at some point, Renderer2D will safely stop using it.
        static CameraWeakPtr s_camera;
    };

} // namespace Renderer
} // namespace Pekan