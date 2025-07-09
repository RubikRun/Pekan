#pragma once

#include "Shape.h"
#include "RenderObject.h"
#include "ISubsystem.h"

namespace Pekan
{
namespace Renderer
{

// Use this macro in your application's main function to include the Renderer2D subsystem of Pekan
#define PEKAN_INCLUDE_SUBSYSTEM_RENDERER2D Pekan::Renderer::Renderer2D::registerSubsystem()

    class Renderer2D : public ISubsystem
    {
        using CameraSharedPtr = std::shared_ptr<Renderer::Camera2D>;
        using CameraWeakPtr = std::weak_ptr<Renderer::Camera2D>;

        friend class Shape;

    public:

        // Registers Renderer2D as a subsystem in Pekan's SubsystemManager,
        // so that it's automatically initialized and exited.
        static void registerSubsystem();

        static void beginFrame();
        static void endFrame();

        // Sets a camera to be used for rendering
        static inline void setCamera(const CameraSharedPtr& camera) { s_camera = camera; }

    private: /* functions */

        void init() override;
        void exit() override;

        ISubsystem* getParent() override;

        bool isInitialized() override { return m_isInitialized; }

        // Submits a shape for rendering.
        // Actual rendering will happen when you call endFrame().
        static void render(const Shape& shape);

    private:

        static RenderObject s_batchDynamic;
        static RenderObject s_batchStatic;

        static std::vector<ShapeVertex> s_verticesDynamic;
        static std::vector<ShapeVertex> s_verticesStatic;
        static std::vector<unsigned> s_indicesDynamic;
        static std::vector<unsigned> s_indicesStatic;


        // A pointer to the camera used for rendering.
        // NOTE: It's a weak pointer so the camera is NOT owned by Renderer2D.
        //       If the camera is destroyed at some point, Renderer2D will safely stop using it.
        static CameraWeakPtr s_camera;

        // A flag indicating if the Renderer2D subsystem is initialized
        bool m_isInitialized = false;
    };

} // namespace Renderer
} // namespace Pekan