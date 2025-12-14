#pragma once

#include "Events/EventListener.h"

#include <glm/glm.hpp>
#include <memory>

namespace Pekan
{
namespace Renderer2D
{

    class Scene2D;

    // A camera controller that allows user to control a 2D scene's camera with the mouse
    class CameraController2D
        : public EventListener
        , public std::enable_shared_from_this<CameraController2D>
    {
        // Declare Scene2D as a friend class to allow it to initialize the camera controller
        friend class Scene2D;

    public:

        // Sets camera controller's zoom speed,
        // determining the amount of zoom to be applied per mouse scroll.
        void setZoomSpeed(float zoomSpeed) { m_zoomSpeed = zoomSpeed; }

        // Sets camera controller's rotation speed,
        // determining the amount of rotation, in radians, to be applied per mouse scroll when Alt key is held down.
        void setRotationSpeed(float rotationSpeed) { m_rotationSpeed = rotationSpeed; }

    private: /* functions */

        // Initializes the camera controller for a given 2D scene.
        // The scene's controllable camera will be controlled by this camera controller.
        // If there are multiple controllable cameras in the scene, the first one found will be controlled.
        void init(Scene2D* scene);

        // Handles a mouse-moved event to move the camera when left mouse button is held down
        bool onMouseMoved(const MouseMovedEvent& event) override;
        // Handles a mouse-scrolled event to zoom the camera in/out when mouse wheel is scrolled
        bool onMouseScrolled(const MouseScrolledEvent& event) override;

    private: /* variables */

        // Zoom speed of the camera controller.
        // Determines the amount of zoom to be applied per mouse scroll.
        float m_zoomSpeed = 1.1f;

        // Rotation speed of the camera controller.
        // Determines the amount of rotation, in radians, to be applied per mouse scroll when Alt key is held down.
        float m_rotationSpeed = 0.03f;

        // Cached mouse position, in window space
        glm::vec2 m_mousePos = { -1.0f, -1.0f };

        // The scene whose camera is being controlled by this camera controller
        Scene2D* m_scene = nullptr;
    };

    typedef std::shared_ptr<CameraController2D> CameraController2D_Ptr;
    typedef std::shared_ptr<const CameraController2D> CameraController2D_ConstPtr;

} // namespace Renderer2D
} // namespace Pekan