#pragma once

#include "ISubsystem.h"
#include "RenderBatch2D.h"

#include <entt/entt.hpp>

namespace Pekan
{
namespace Renderer2D
{

// Use this macro in your application's main function to include the Renderer2D subsystem of Pekan
#define PEKAN_INCLUDE_SUBSYSTEM_RENDERER2D Pekan::Renderer2D::Renderer2DSubsystem::registerSubsystem()

    class Renderer2DSubsystem : public ISubsystem
    {

        friend class Shape;
        friend class Sprite;

    public:

        std::string getSubsystemName() const override { return "Renderer2D"; }

        // Registers Renderer2DSubsystem as a subsystem in Pekan's SubsystemManager,
        // so that it's automatically initialized and exited.
        static void registerSubsystem();

        // To be called at the beginning of every 2D scene's render() function.
        static void beginFrame();
        // To be called at the end of every 2D scene's render() function.
        static void endFrame();

        // Returns (a const pointer to) the camera currently used for rendering
        static Camera2D_ConstPtr getCamera() { return s_camera.lock(); }
        // Sets a camera to be used for rendering
        static void setCamera(const Camera2D_ConstPtr& camera) { s_camera = camera; }

        // TODO: this function has to be removed once ECS is fully integrated
        // Returns current mouse position in world space, using current camera
        static glm::vec2 getMousePosition();

        // TODO: this function has to be renamed, removing the "_ECS" suffix, once ECS is fully integrated
        // Returns current mouse position in world space, using scene's primary camera
        static glm::vec2 getMousePosition_ECS(const entt::registry& registry);

    private: /* functions */

        bool init() override;
        void exit() override;

        ISubsystem* getParent() override;

        // Submits a shape for rendering.
        // Actual rendering will happen later.
        static void submitForRendering(const Shape& shape);
        // Submits a sprite for rendering.
        // Actual rendering will happen later.
        static void submitForRendering(const Sprite& sprite);

    private:

        // A batch of 2D primitives that have been submitted for rendering, and not yet rendered.
        static RenderBatch2D s_batch;

        // A pointer to the camera used for rendering.
        // NOTE: It's a weak pointer so the camera is NOT owned by Renderer2D.
        //       If the camera is destroyed at some point, Renderer2D will safely stop using it.
        static Camera2D_ConstWeakPtr s_camera;
    };

} // namespace Renderer2D
} // namespace Pekan