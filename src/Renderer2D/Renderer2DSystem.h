#pragma once

#include "ISubsystem.h"
#include "ShapesBatch.h"
#include "SpritesBatch.h"

namespace Pekan
{
namespace Renderer2D
{

// Use this macro in your application's main function to include the Renderer2D subsystem of Pekan
#define PEKAN_INCLUDE_SUBSYSTEM_RENDERER2D Pekan::Renderer2D::Renderer2DSystem::registerSubsystem()

    class Renderer2DSystem : public ISubsystem
    {

        friend class Shape;
        friend class Sprite;

    public:

        std::string getSubsystemName() const override { return "Renderer2D"; }

        // Registers Renderer2DSystem as a subsystem in Pekan's SubsystemManager,
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

        // Returns current mouse position in world space, using current camera
        static glm::vec2 getMousePosition();

    private: /* functions */

        bool init() override;
        void exit() override;

        ISubsystem* getParent() override;

        // Submits a shape for rendering.
        // Actual rendering will happen when you call endFrame()
        // or when the shapes batch is filled up.
        static void render(const Shape& shape);

        // Renders all shapes from a given batch with current camera
        static void renderShapesBatch(ShapesBatch& batch);

        // Submits a sprite for rendering.
        // Actual rendering will happen when you call endFrame()
        // or when the sprites batch is filled up.
        static void render(const Sprite& sprite);

        // Renders all sprites from a given batch with current camera
        static void renderSpritesBatch(SpritesBatch& batch);

    private:

        // A batch of dynamic (often moved/transformed) shapes
        static ShapesBatch s_shapesBatchDynamic;
        // A batch of static (not moved/transformed much) shapes
        static ShapesBatch s_shapesBatchStatic;

        // A batch of dynamic (often moved/transformed) sprites
        static SpritesBatch s_spritesBatchDynamic;
        // A batch of static (not moved/transformed much) sprites
        static SpritesBatch s_spritesBatchStatic;

        // A pointer to the camera used for rendering.
        // NOTE: It's a weak pointer so the camera is NOT owned by Renderer2D.
        //       If the camera is destroyed at some point, Renderer2D will safely stop using it.
        static Camera2D_ConstWeakPtr s_camera;
    };

} // namespace Renderer2D
} // namespace Pekan