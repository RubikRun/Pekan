#pragma once

#include "Camera2D.h"

#include <memory>

namespace Pekan
{
namespace Tools
{

    // A singleton/static class containing game development tools
    class PekanTools
    {

    public:

        // Enables camera controller for a given 2D camera,
        // allowing user to control the camera with the mouse in a standard way:
        // - Hold down left button + Move mouse = Move camera left/right/up/down
        // - Scroll up/down = Zoom camera in/out
        // 
        // NOTE: Only a single 2D camera can be controlled at a time,
        // so if another camera was being controlled until now,
        // the control will be transfered to the new camera.
        static void enableCameraController2D(const Renderer2D::Camera2D_Ptr& camera);

        // Disables camera controller for the currently controlled 2D camera
        static void disableCameraController2D();

        // Sets zoom speed for the 2D camera controller
        static void setCameraController2DZoomSpeed(float zoomSpeed);
    };

} // namespace Tools
} // namespace Pekan