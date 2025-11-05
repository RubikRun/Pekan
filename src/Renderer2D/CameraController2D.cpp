#include "CameraController2D.h"

#include "Scene2D.h"
#include "Events/MouseEvents.h"
#include "CameraSystem2D.h"
#include "PekanEngine.h"
#include "PekanApplication.h"
#include "PekanLogger.h"

#include <memory>

namespace Pekan
{
namespace Renderer2D
{

    void CameraController2D::init(Scene2D* scene)
    {
        if (scene == nullptr)
        {
            PK_LOG_ERROR("Trying to initialize CameraController2D with a null Scene2D pointer.", "Pekan");
            return;
        }
        m_scene = scene;

        // Get current application
        PekanApplication* application = PekanEngine::getApplication();
        if (application == nullptr)
        {
            PK_LOG_ERROR("Trying to initialize CameraController2D, but there is no application registered in PekanEngine.", "Pekan");
            return;
        }

        // Register this camera controller as an event listener in the application
        application->registerEventListener(shared_from_this());
    }

    bool CameraController2D::onMouseMoved(const MouseMovedEvent& event)
    {
        if (m_scene == nullptr)
        {
            PK_LOG_ERROR("CameraController2D received a mouse-moved event, but it's not initialized.", "Pekan");
            return false;
        }
        // Get controllable camera from the scene's registry
        CameraComponent2D* camera = CameraSystem2D::getControllableCamera(m_scene->getRegistry());
        if (camera == nullptr)
        {
            return false; // if there is no controllable camera, do nothing
        }

        // Get new mouse position, in window space, from the event
        const glm::vec2 newMousePos = { event.getX(), event.getY() };

        // Check if left mouse button is held down for dragging
        if (PekanEngine::getWindow().isMouseButtonPressed(MouseButton::Left))
        {
            // Calculate how much mouse has moved, in window space
            const glm::vec2 mouseDelta = newMousePos - m_mousePos;
            // Calculate how much mouse has moved, in world space
            glm::vec2 worldDelta = camera->windowToWorldSize(mouseDelta);
            // Flip Y axis because window's Y axis points downwards, but world's Y axis points upwards
            worldDelta.y = -worldDelta.y;
            // Move camera in opposite direction of mouse movement for intuitive panning
            camera->move(-worldDelta);
        }

        // Update cached mouse position
        m_mousePos = newMousePos;
        
        return true;
    }

    bool CameraController2D::onMouseScrolled(const MouseScrolledEvent& event)
    {
        if (m_scene == nullptr)
        {
            PK_LOG_ERROR("CameraController2D received a mouse-scrolled event, but it's not initialized.", "Pekan");
            return false;
        }
        // Get controllable camera from the scene's registry
        CameraComponent2D* camera = CameraSystem2D::getControllableCamera(m_scene->getRegistry());
        if (camera == nullptr)
        {
            return false; // if there is no controllable camera, do nothing
        }

        // Get mouse position in world space before zooming
        const glm::vec2 mousePosWorldBefore = camera->windowToWorldPosition(m_mousePos);

        // Get scroll amount from the event (*1)
        const float scrollAmount = event.getYOffset();
        // Apply zoom based on scroll amount
        if (scrollAmount > 0.0f)
        {
            camera->zoomIn(m_zoomSpeed);
        }
        else if (scrollAmount < 0.0f)
        {
            camera->zoomOut(m_zoomSpeed);
        }

        // Use the difference between mouse position in world space before zooming,
        // and mouse position in world space after zooming,
        // to move the camera, so that zooming is done in the direction of the mouse, NOT towards the center.
        const glm::vec2 mousePosWorldAfter = camera->windowToWorldPosition(m_mousePos);
        const glm::vec2 cameraPosDelta = mousePosWorldBefore - mousePosWorldAfter;
        camera->move(cameraPosDelta);

        return true;
    }




    ///// Notes /////
    // (*1): Scroll amount will be mostly 1.0 or -1.0.
    //       If scrolled really fast it could sometimes be 2.0 or -2.0.
    //       We could multiply the zoom speed by this scroll amount to achieve faster zoom when scrolling faster,
    //       but it doesn't feel natural. It feels better to have a constant zoom speed no matter how fast you scroll.
    //////////////////

} // namespace Renderer2D
} // namespace Pekan