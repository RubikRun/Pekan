#include "PekanTools.h"

#include "PekanLogger.h"
#include "PekanEngine.h"
#include "PekanApplication.h"
#include "Events/MouseEvents.h"
#include "Events/EventListener.h"

static const float DEFAULT_CAMERA_CONTROLLER_2D_ZOOM_SPEED = 1.1f;

using namespace Pekan::Renderer2D;

namespace Pekan
{
namespace Tools
{


    /////////////////////////////////////////////////////////////////////////////////
    ///////////////////////// class CameraController2D //////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////


    class CameraController2D : public EventListener
    {

    public:

        CameraController2D() = default;

        bool onMouseMoved(const MouseMovedEvent& event) override
        {
            if (!m_enabled)
            {
                return false;
            }

            Camera2D_Ptr camera = m_camera.lock();
            // Check if camera still exists
            if (camera == nullptr)
            {
                return false;
            }

            // Get window's size
            const glm::vec2 windowSize = glm::vec2(PekanEngine::getWindow().getSize());

            // Calculate how much the mouse has moved from its previous position to the current position
            const glm::vec2 newMousePos = { event.getX(), event.getY() };
            const glm::vec2 mouseDelta = newMousePos - m_mousePos;
            // Update cached mouse position
            m_mousePos = newMousePos;

            if (PekanEngine::isMouseButtonPressed(MouseButton::Left))
            {
                // Mouse coordinates are in window space, we need them in world space.
                // We can divide the camera's size by the window's size multiplied by zoom level to get a vector
                // that can be used to multiply any window-space coordinate to get a world-space coordinate.
                const glm::vec2 windowToCameraFactor = camera->getSize() / (windowSize * camera->getZoom());
                // Move camera by the amount that the mouse has moved in world space
                camera->move(glm::vec2(-mouseDelta.x, mouseDelta.y) * windowToCameraFactor);
            }

            return true;
        }

        bool onMouseScrolled(const MouseScrolledEvent& event) override
        {
            if (!m_enabled)
            {
                return false;
            }

            Camera2D_Ptr camera = m_camera.lock();
            // Check if camera still exists
            if (camera == nullptr)
            {
                return false;
            }

            const glm::vec2 mousePosWorldBefore = camera->windowToWorld(m_mousePos);

            // NOTE: Scroll amount will be mostly 1.0 or -1.0.
            //       If scrolled really fast it could sometimes be 2.0 or -2.0.
            //       We could multiply the zoom speed by this scroll amount to achieve faster zoom when scrolling faster,
            //       but it doesn't feel natural. It feels better to have a constant zoom speed no matter how fast you scroll.
            const float scrollAmount = event.getYOffset();
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
            const glm::vec2 mousePosWorldAfter = camera->windowToWorld(m_mousePos);
            const glm::vec2 cameraPosDelta = mousePosWorldBefore - mousePosWorldAfter;
            camera->move(cameraPosDelta);

            return true;
        }

        // Enables/disables the camera controller
        inline void setEnabled(bool enabled) { m_enabled = enabled; }

        // Sets a new camera to be controlled by the camera controller
        inline void setCamera(const Camera2D_Ptr& camera) { m_camera = camera; }

        // Sets zoom speed for the 2D camera controller
        inline void setZoomSpeed(float zoomSpeed) { m_zoomSpeed = zoomSpeed; }

    private:

        // A pointer to the camera being controlled by the camera controller.
        // NOTE: It's a weak pointer so the camera is NOT owned by the camera controller.
        //       If the camera is destroyed at some point, the camera controller will safely stop operating on it.
        Camera2D_WeakPtr m_camera;

        // Cached mouse position, in window space
        glm::vec2 m_mousePos = glm::vec2(-1.0f, -1.0f);

        // A flag indicating if camera controller is currently enabled
        bool m_enabled = true;

        // Camera controller's zoom speed.
        // Determines how much zoom will be done per mouse scroll.
        float m_zoomSpeed = DEFAULT_CAMERA_CONTROLLER_2D_ZOOM_SPEED;
    };

    // A global static 2D camera controller.
    // 
    // WARNING: The logic here rests on the fact that we support only 1 application at a time.
    //          If sometime in the future we support more than 1 application at a time,
    //          we'd need to rework this to be a map from application ID to 2D camera controller.
    static std::shared_ptr<CameraController2D> s_cameraController2D = std::make_shared<CameraController2D>();

    // A flag indicating if we have already registered the 2D camera controller as an event listener inside the application
    static bool s_didRegisterCameraController2D = false;


    /////////////////////////////////////////////////////////////////////////////////
    ///////////////////////// PekanTools functions //////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////


	void Tools::PekanTools::enableCameraController2D(const Camera2D_Ptr& camera)
	{
        s_cameraController2D->setCamera(camera);
        s_cameraController2D->setEnabled(true);

        if (!s_didRegisterCameraController2D)
        {
            PekanApplication* application = PekanEngine::getApplication();
            if (!application)
            {
                PK_LOG_ERROR("Trying to enable 2D camera controller but there is no application registered in PekanEngine", "Pekan");
                return;
            }
            application->registerEventListener(s_cameraController2D);
            s_didRegisterCameraController2D = true;
        }
	}

	void PekanTools::disableCameraController2D()
	{
        s_cameraController2D->setEnabled(false);
	}

    void PekanTools::setCameraController2DZoomSpeed(float zoomSpeed)
    {
        s_cameraController2D->setZoomSpeed(zoomSpeed);
    }

} // namespace Tools
} // namespace Pekan